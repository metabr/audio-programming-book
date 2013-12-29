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
/* varargdemo.c */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* GNU: The type of the value returned by va_arg is type as specified in the call. 
  type must be a self-promoting type (not char or short int or float)
  that matches the type of the actual argument.
  */
/* about va_start

#define va_start(va_list ap, last-par) <void expression>

The macro stores initial context information in the object designated by ap. 
last-par is the name of the last parameter you declare. For example, last-par is b 
for the function declared as int f(int a, int b, ...). The last parameter must 
not have register storage class, and it must have a type that is not changed by 
the translator. It cannot have:

    * an array type
    * a function type
    * type float
    * any integer type that changes when promoted 

*/



enum {ID_ONEARG,ID_2ARGS,ID_3ARGS};

typedef struct complex {
	double re;
	double im;
} COMPLEX;

void vafunc(int id, ...);

double addfunc(unsigned int nargs,...);

/* decide that:

	ONEARG:  int id, float farg
	2ARGS:	int id, COMPLEX carg,double darg
	3ARGS:   int id, float farg, double darg, char* str
*/
void vafunc( int id, ...)
{
	double darg;
	COMPLEX carg;
	const char* strarg;
	va_list args;
	
	va_start(args,id);
	/* NB: all floats promoted to doubles by ... */
	switch(id){
	case(ID_ONEARG):
		darg = va_arg(args,double);
		printf("func called with 2 args: arg 2 = %f\n",darg);
		break;
	case(ID_2ARGS):
		carg =  va_arg(args,COMPLEX);
		darg =va_arg(args,double); 
		printf("func called with 3 args: arg 2 = %f:%f, arg3 = %f\n",carg.re,carg.im,darg);
		break;
	case(ID_3ARGS):		
		darg = va_arg(args,double);
		carg =  va_arg(args,COMPLEX);
		strarg = va_arg(args,char*);
		printf("func called with 4 args: arg 2 = %f, arg3 = %f:%f\n\targ4 = %s\n",darg,carg.re,carg.im,strarg);
		break;
	default:
		printf("undefined function argument list\n");
		break;
	}
	va_end(args);
}

double addfunc(unsigned int nargs,...)
{
	double sum = 0.0;
	unsigned int i;
	va_list args;
	
	va_start(args,nargs);
    /* this is not portable, nor necessarily correct,
        but interesting "as an exercise": va_list may not always be an array type.
        See the compiler error msgs (if any).
    */
	if(*args==0)	 
		printf("no optional args\n");
	
	for(i=0; i < nargs;i++)
		sum += va_arg(args,double);
	va_end(args);
	return sum;
}

int main(void)
{
	float farg = 0.123f;
	double darg = 1.0594631;
	COMPLEX carg = {0.123,0.456};
	
	vafunc(ID_ONEARG,farg);
	vafunc(ID_2ARGS,carg,darg);
	vafunc(ID_3ARGS,darg,carg,"this is the third argument\n");						   
	darg = addfunc(4,0.1,0.2,0.3,0.5);
	printf("sum = %f\n",darg);
	darg = addfunc(0);
	printf("sum = %f\n",darg);
	darg = addfunc(3,1,2,4);	 /* error, type mismatch */
	printf("sum = %f\n",darg);
	return 0;
}

