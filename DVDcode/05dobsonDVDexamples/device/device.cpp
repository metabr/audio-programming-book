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
#include <cassert>
#include <iostream>
#include "device.h"
using namespace USB_DEVICE;

// play with this ad lib to define this or that device
bool deviceAdded(USBDevice** newdev){
	assert(newdev); // programming error to send bad pointer
  bool res(false);	
  try{
		*newdev = new ExtAudioDevice();
		res = true;
		std::cout << " New USB device detected. Configuring...\n";
		}
  catch(...) {
		std::cout << "faulty hardware!\n";		
	}
	return res;
}

const char* deviceTypeName[] = {"unknown","Storage Device",
  "Audio Device","Midi Device","Keyboard","Mouse"};

int main()
{
  // all calls can go through base class interface  
 	USBDevice* dev(0);
  if(deviceAdded(&dev)){
    unsigned int nDevices = dev->getNumDevices();
    std::cout << "hardware supports " << nDevices << " devices\n"; 
    for(unsigned int device = 0; device < nDevices; device++){
      USBType type = dev->QueryDeviceType(device);
      int prop;
      switch(type){
        case USB_AUDIO:
          std::cout << deviceTypeName[type] << std::endl;          
          std::cout << "name  = " << dev->getName(device) 
          << std::endl;
          prop = dev->getDeviceProp(device,USBA_CURSR);
          if(prop >=0)
            std::cout << "Srate = " << prop << std::endl; 
            prop = dev->getDeviceProp(device,USBA_INCHANS);
          if(prop >=0)
            std::cout << "Audio Inputs: "<< prop << std::endl;
            prop = dev->getDeviceProp(device,USBA_OUTCHANS);
          if(prop >=0)
            std::cout << "Audio Outputs: "<< prop << std::endl;
            break;
        case USB_MIDI:
          std::cout << deviceTypeName[type] << std::endl;
          std::cout << "name  = " << dev->getName(device) 
          << std::endl;
          prop = dev->getDeviceProp(device,USBB_INPORTS);
          if(prop >=0)
            std::cout << "MIDI Inputs: " << prop << std::endl;
            prop = dev->getDeviceProp(device,USBB_OUTPORTS);
          if(prop >=0)
            std::cout << "MIDI Outputs: " << prop << std::endl;
            break;
        default:
          std::cout << "Unrecognised Device!\n";
          break;
      }
      std::cout << "Your new hardware is ready for use.\n";
    }
	}
	return 0;
}

