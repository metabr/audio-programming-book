/*
 *  SMS_DebugMacros.h
 *  SMS_Controller
 *
 *  Created by Taemin Cho on 7/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SMS_DEBUG_H_
#define _SMS_DEBUG_H_

static int _err = noErr; // noErr = 0, defined in MacTypes.h

static const char * _NAME(const char * inStr) { 
	int count = strlen(inStr); 
	while (count && *(inStr + count - 1) != '/')
		count--;
	return inStr + count;
} 

#define ERR( x )																					\
	do{																								\
		_err = (x);																					\
		if ( _err )																					\
			fprintf(stderr, "%s:%d:%s -> Error %d\n", _NAME(__FILE__), __LINE__, __func__, _err);	\
	}while(0)

#endif	/* _SMS_DEBUG_H_ */
