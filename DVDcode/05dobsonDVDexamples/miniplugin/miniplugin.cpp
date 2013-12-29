/* Copyright (c) 2009 Richard Dobson

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

// miniplugin.cpp
// create a concrete plugin based on mini_plugin
#include <iostream>
#include <cstring>
#include "miniplugin.h"
#include "vdelayb.h"

using namespace MiniPlugin;

class delayPlugin : public mini_plugin
{
public:
	delayPlugin(minihostCallback callback);
	virtual ~delayPlugin() {}
	// we must implement all mini_plugin methods unless
    // default versions will do, and override any we want to!
	virtual uint_t get_nChannels()       const { return 2;}
	virtual uint_t get_api_version() const {return MPV_VERSION_1;}
	virtual const char* get_pluginName()   
                                const { return "Mini variDelay";} 
	virtual const char* get_paramname(uint_t param) 
                                  const {return "Delay Time";}
	virtual float get_parameter(uint_t param)     
                          const { return fDelayms_ / 1000.0f; }
	virtual void  set_parameter(float val, uint_t param) 
                                 { fDelayms_ = val * 1000.0f; }	 
	virtual const char* get_paramtext(uint_t param) 
                                 const {return "ms"; }
	virtual void set_srate(uint_t srate);
	virtual void prepare_process();
	virtual void process(float** inbufs, float** outbufs,
                                  unsigned int blocksize); 
private:
    enum { MAXDELAY = 100}; // hard-wired max delay time (ms)
    audiobook::vdelayb vd[2];
	bool initialized;
	float fMaxDur_;
	float fDelayms_; // value in msecs
};

delayPlugin::delayPlugin(minihostCallback callback) 
                     : mini_plugin(callback,1),initialized(false)
{
    MINIHOST info; // could be a gainPlugin class variable
    // send info requests to Host
    int res = callback( MH_GET_HOSTNAME,&info); 
    if(res)
	    std::cout << "host name = " << info.hostname << std::endl;
    res = callback(MH_GET_VERSION, &info);
    if(res)
	     std::cout << "OK: host supports API version " 
                   << info.version << std::endl; 
    // supply a default srate
	set_srate(48000);
			 
}

void delayPlugin::set_srate(uint_t sr)
{
	if(sr != srate_){
        // we complete reinit in prepare_process
        initialized = false;   
        srate_ = sr;
	}
}

void delayPlugin::prepare_process()
{
    if(!initialized) {	   
		vd[0].init(srate_,MAXDELAY * 0.001);
		vd[1].init(srate_,MAXDELAY * 0.001);
		initialized = true;
	}
	vd[0].reset();
	vd[1].reset();
}

void delayPlugin::process(float** inbufs, float** outbufs, 
                                unsigned int blocksize){
	float* in1  =  inbufs[0];
    float* in2  =  inbufs[1];
    float* out1 = outbufs[0];
    float* out2 = outbufs[1];
	// access params once only before processing
	// convert value in msecs to secs
    // make channels slightly different
	float thisdelay1  = fDelayms_  * 0.001;
    float thisdelay2  = thisdelay1 * 0.9;
    
    for(unsigned int i = 0; i < blocksize; i++){
        (*out1++) = vd[0].tick(*in1++,thisdelay1,0.0);
        (*out2++) = vd[1].tick(*in2++,thisdelay2,0.0);
    }
}

/****** test program: fake host and plugin in one process! ***/
static  char* hostname = "MIT Mini-Host";
// we tell host what API version we use
static  int hostVersion = MPV_VERSION_1;
// global host mini callback function
// pointer to a struct that can contain pretty much anything  
int hcallback(minihostCmd cmd, MINIHOST* pHost)
{   
    int res(0);
	// plugin is requesting this information...
	switch(cmd){
	case MH_GET_VERSION:
	    pHost->version = hostVersion;
		res = 1;
		break;
	case MH_GET_HOSTNAME:
	    strcpy(pHost->hostname,hostname);
		res = 1;
		break;
	}
	return res;
}
// pretend this is a host application!
int main(int argc, char* argv[])
{
    // for some test data - don't know how many channels yet...
    float **inbuf, **outbuf;
	uint_t nParams, nChannels;
	uint_t blocksize(32);

    // in a real system, we would just get pointer to mini_plugin
	// via a function that loads the plugin from a shared library.
	// NB: constructor reports host into 
	mini_plugin* plugin = new delayPlugin(&hcallback);
	//  check version!
	int ver = plugin->get_api_version();
	if(ver != MPV_VERSION_1) {
	    std::cout << "Error: unsupported API version!\n";
		return 1;
	}
	const char* name = plugin->get_pluginName();
	std::cout << "Plugin " << name << " created OK \n";
	
	nParams = plugin->get_nParams();
	name    = plugin->get_paramname(0);
	std::cout << "it has " << nParams << " parameters\n";
	std::cout << "parameter 1 = " << name << std::endl;
	nChannels = plugin->get_nChannels();
	std::cout << " creating " << nChannels << "-channel stream\n";
	/* now we can setup nChannels buffers */
    /* no interleaved audio here */
	inbuf  = new float* [nChannels];
	outbuf = new float* [nChannels];
	for(uint_t i = 0; i < nChannels; i++){
	    inbuf[i]  = new float[blocksize];
		outbuf[i] = new float[blocksize];
        /* make some simple test data */
		for(uint_t j = 0;j < blocksize; j++){
		    inbuf[i][j]  = static_cast<float>(j / 
                            static_cast<float>(blocksize));
			outbuf[i][j] = 0.0;
		}
	}
	std::cout << "preparing to process: \n";	
	// host is required to issue plugin setup commands 
    // before running process
	plugin->set_srate(44100); 
	plugin->prepare_process();
	// set a very short delay for testing: 16 samps in msecs!
	float delay16 = (16.0f/44100.0f);
	// this could be called at any time, from background  thread
	plugin->set_parameter(delay16, 0);
	// host would call this repeatedly from high-priority 
    // thread for each new sample block
	plugin->process(inbuf, outbuf, blocksize);
	// check output on both chans and clean up
    // make tidy formatting
    std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    std::cout.precision(4);
	for(uint_t i = 0; i < blocksize; i++){
	    std::cout << inbuf[0][i] << "\t--> " <<  outbuf[0][i] 
                  << "\t:" << outbuf[1][i] << std::endl;
	}	
	for(uint_t i = 0; i < nChannels; i++){
	    delete [] inbuf[i];
		delete [] outbuf[i];
	}	 
	delete plugin;
	return 0;
}
// issue to consider: a plugin is expected to load
// with default settings for all params, so is
// immediately ready to process
