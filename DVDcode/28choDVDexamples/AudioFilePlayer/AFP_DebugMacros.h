/*
 *  AFP_DebugMacros.h
 *  SMS_Controller
 *
 *  Created by Taemin Cho on 1/10/08.
 *  Copyright 2008 __Taemin Cho__. All rights reserved.
 *
 */

#ifndef _AFP_DEBUG_H_
#define _AFP_DEBUG_H_

static int _err = noErr; // noErr = 0, defined in MacTypes.h

static const char * _NAME(const char * inStr) { 
	int count = strlen(inStr); 
	while (count && *(inStr + count - 1) != '/')
		count--;
	return inStr + count;
} 

#define RETURN_ERR( x )																							\
			do{																									\
				_err = (x);																						\
					if ( _err ){																				\
						fprintf(stderr, "%s:%d:%s -> Error %d\n", _NAME(__FILE__), __LINE__, __func__, _err);	\
						goto RETURN;																			\
					}																							\
			}while(0)

#define ERR( x )																								\
			do{																									\
				_err = (x);																						\
					if ( _err )																					\
						fprintf(stderr, "%s:%d:%s -> Error %d\n", _NAME(__FILE__), __LINE__, __func__, _err);	\
			}while(0)

#endif	

/* _AFP_DEBUG_H_ */