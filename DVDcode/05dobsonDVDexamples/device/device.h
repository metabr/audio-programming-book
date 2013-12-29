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

// device.h : interface of putative plugin audio device
#ifndef __DEVICE_H__
#define __DEVICE_H__

namespace USB_DEVICE {

typedef enum USBType {USB_ERR_TYPE,USB_DISK, USB_AUDIO, 
                      USB_MIDI, USM_KEYBOARD, USB_MOUSE};

enum USB_AudioProps {USBA_INCHANS,USBA_OUTCHANS,
        USBA_MINSR,USBA_MAXSR,USBA_CURSR,USBA_DEFSR};
enum USB_MidiProps { USBB_INPORTS,USBB_OUTPORTS};    
typedef  unsigned int uint_t;

// strictly interface: no state defined, only required behavior
class USBDevice {
public:	    
    virtual ~USBDevice() {}
    virtual uint_t getNumDevices() const = 0;
    virtual USBType QueryDeviceType(uint_t type) const = 0;
    virtual const char* getName(uint_t nDev) const = 0;
    virtual int getDeviceProp(
                uint_t device,uint_t prop) const = 0;
    virtual int setDeviceProp(uint_t device, uint_t prop) = 0;
protected:
    // can only be created via derived class
	USBDevice() {}
};
// define usb device with audio and MIDI	
class ExtAudioDevice : public USBDevice {
public:
	ExtAudioDevice() : nDevices_(2),
    nInputs_(8), nOutputs_(8), 
    default_srate_(96000), cur_srate_(default_srate_),
    min_srate_(11025), max_srate_(192000),
    nMidiInputs_(2),  nMidiOutputs_(2)  { }
	~ExtAudioDevice() {}
	uint_t getNumDevices()    const { return 2;}
	USBType QueryDeviceType(uint_t device) const
    {
        USBType utype(USB_ERR_TYPE);
		if(device==0) 
			utype =  USB_AUDIO;
		else if(device==1)
			utype = USB_MIDI;
		return utype;		
	}
    const char* getName(uint_t nDev) const 
    { 
        if(nDev==0) return "MIT Pro Audio Device";
        else if(nDev==1) return "MIT Pro Midi Device";
        else return 0; 
    }
    int getDeviceProp(uint_t device,uint_t devprop) const {
        int prop(-1);
        switch(device){
            case 0:  // Audio
                switch(devprop){
                    case USBA_INCHANS: 
                        prop = nInputs_; break;
                    case USBA_OUTCHANS:
                        prop = nOutputs_; break;
                    case USBA_MINSR:
                        prop = min_srate_; break;
                    case USBA_MAXSR:
                        prop = max_srate_; break; 
                    case USBA_CURSR:
                         prop = cur_srate_; break;
                    case USBA_DEFSR:
                         prop = default_srate_; break;
                    default:
                        break;
                }
                break;
            case 1:// MIDI
                switch(devprop){
                    case USBB_INPORTS:
                        prop = nMidiInputs_; break;
                    case USBB_OUTPORTS:
                        prop = nMidiOutputs_; break;
                    default:
                        break;
                }
            default:
                break;
        }
        return prop;
    }
    // placeholder!
    int setDeviceProp(uint_t device,uint_t prop){
        int res = 0;
        if(device > 1)
            res = -1;
        return res;
    }
    private:
    uint_t nDevices_;
    uint_t nInputs_,nOutputs_;
	uint_t default_srate_, cur_srate_;
    uint_t min_srate_,max_srate_;
    uint_t nMidiInputs_;
    uint_t nMidiOutputs_;
};
    
}
#endif