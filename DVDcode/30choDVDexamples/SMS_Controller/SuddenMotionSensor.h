/*
 *  SuddenMotionSensor.h
 *  SMS_Controller
 *
 *  Created by Taemin Cho on 7/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>

typedef struct {
	const char *    name;	// Kernel Extension file Name.
	uint32_t		index;	// index of the mothod
	size_t			structureInputSize;     // the size of the output struct
	size_t			structureOutputSize;	// the size of the input struct
	int				data_type;	// the size of the output data type
}sms_kexts_t;

typedef struct {
	int	x;			// Sudden Motion Sensor x axis value
	int	y;			// Sudden Motion Sensor x axis value
	int z;			// Sudden Motion Sensor x axis value
}sms_data_t;

int sms_Open(void);	 // find a Sudden Motion Sensor and make the connection.
int sms_GetData(sms_data_t* smsData); // Get data from the Sudden Motion Sensor.
int sms_Close(void); // close the connection to the Sudden Motion Sensor.