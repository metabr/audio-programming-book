/*
 *  SuddenMotionSensor.c
 *  SMS_Controller
 *
 *  Created by Taemin Cho on 7/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SuddenMotionSensor.h"

static mach_port_t dataPort;

static sms_kexts_t sms_kexts_list[] = {
	{"IOI2CMotionSensor",	21,	60, 60, sizeof(int8_t)},	// PowerBook G4, iBook G4
	{"PMUMotionSensor",		21, 60, 60, sizeof(int8_t)},	// Hi Res Powerbook
	{"SMCMotionSensor",		5,  40, 40, sizeof(int16_t)},	// MacBook, MacBookPro
	{NULL,					0,   0,  0, 0}					// end of the list
};

static sms_kexts_t* sms_kext_index;

int sms_Open(void){
	
	kern_return_t	kernResult;
	io_service_t	smsDevice;
	
	for (sms_kext_index = sms_kexts_list;sms_kext_index->name ; sms_kext_index++){
		
		// Create a dictionary to match on SMS Object
		CFMutableDictionaryRef mySMSMatchingDictionary = IOServiceMatching(sms_kext_index->name);
		if (!mySMSMatchingDictionary)
			return 1; // error
		
		// Get a SMS Object
		smsDevice = IOServiceGetMatchingService(kIOMasterPortDefault, mySMSMatchingDictionary);
		
		if(smsDevice) 
			break;
	}
	
	//Create a SMS User Connection
	kernResult = IOServiceOpen(smsDevice, mach_task_self(), 0, &dataPort);
	IOObjectRelease(smsDevice); // Release the SMS Object
	
	if (kernResult != KERN_SUCCESS)
		return 2; // error
	
	return 0;
}


int sms_GetData(sms_data_t* smsData){
	
	int8_t* type1;
	int16_t* type2;
	
	void*	inputStructure = alloca(sms_kext_index->structureInputSize);
	void*	outputStructure = alloca(sms_kext_index->structureOutputSize);
	
	kern_return_t	kernResult;
	
	kernResult =
		IOConnectCallStructMethod(dataPort,	// an io_connect_t returned from IOServiceOpen().
								  sms_kext_index->index,			// an index to the function to be called via the user client.
								  inputStructure,	// a pointer to the input struct parameter.
								  sms_kext_index->structureInputSize,	// the size of the input struct paramter.
								  outputStructure,	// a pointer to the output struct parameter.
								  &(sms_kext_index->structureOutputSize)	// a pointer to the size of the output struct paramter.
								  );
	
	if (kernResult != KERN_SUCCESS)
		return 3; // error
	
	switch(sms_kext_index->data_type){
		case 1:
			type1 = outputStructure;
			smsData->x = *type1++ * 5;
			smsData->y = *type1++ * 5;
			smsData->z = *type1 * 5;
			break;
		case 2:
			type2 = outputStructure;
			smsData->x = *type2++ ;
			smsData->y = *type2++ ;
			smsData->z = *type2 ;
			break;
		default:
			return 4; // error
	};
	
	return 0;
}

int sms_Close(void){
	
	kern_return_t	kernResult;
	
	kernResult = IOServiceClose(dataPort);
	
	if (kernResult != KERN_SUCCESS)
		return 5; // error
	
	return 0;
}