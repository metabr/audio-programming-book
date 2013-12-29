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

/* miniplugin.h: definition of small plugin API class */
#ifndef __MINIPLUGIN_H__
#define __MINIPLUGIN_H__

namespace MiniPlugin {
// add more host commands here
typedef enum minihostCmd {MH_GET_VERSION = 0,MH_GET_HOSTNAME};  
typedef struct minihost {
    unsigned int version;  /* API version supported by the host */
    char hostname[32];
   /* anything else we think we need to know?*/   
} MINIHOST;

typedef unsigned int uint_t;
// callback must be implemented by host, to pass to plugin
extern "C" { 
    typedef int (*minihostCallback)
                 (minihostCmd cmd, MINIHOST* pHost); 
}
// plugin API version
enum  {MPV_VERSION_1 = 100};
// the plugin base class	
class mini_plugin {
public:
    //we are making a plugin object we then pass back 
    // to the host, so we know about ourselves
    mini_plugin(minihostCallback mh_callback,uint_t nPluginParams) 
        : callback_(mh_callback), nParams_(nPluginParams) {}
    virtual ~mini_plugin()                   {}
    // everything the host needs to know about this plugin!
    virtual uint_t get_api_version()        const = 0;
    virtual uint_t get_nParams()    const  { return nParams_;}
    virtual uint_t get_nChannels()          const = 0;
    virtual const char* get_pluginName()    const = 0;
    virtual const char* get_paramname(uint_t param) const = 0; 
    //**** background control methods. 
    virtual void set_srate(uint_t srate)   {srate_ = srate;}
    // initialise effect (incl memory allocations)
    virtual void prepare_process() {} 
    // tells plugin audio has been stopped or suspended
    virtual void stop()            {}  
    // tell plugin it can do any background work now
    virtual void run_background()  {}  
    // *****  dynamic (runtime) methods
    virtual float get_parameter(uint_t param)       const = 0;
    virtual void  set_parameter(float val, uint_t param)  = 0;
    virtual const char* get_paramtext(uint_t param) const = 0; 
    // supply non-interleaved buffers 
    virtual void process(float** inbufs, float** outbufs,
                                        uint_t blocksize) = 0; 
protected:
    // called by derived class on creating the plugin
    virtual void set_nInputs(uint_t inputs)   
                { nInputs_ = inputs;}
    virtual void set_nOutputs(uint_t outputs) 
                { nOutputs_ = outputs;}
    const minihostCallback callback_;
    uint_t srate_;
    uint_t nParams_;
    uint_t nInputs_;
    uint_t nOutputs_;
};

} // namespace

#endif
		