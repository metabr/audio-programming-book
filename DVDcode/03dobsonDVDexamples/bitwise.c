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
/* bitwise.c. Demo program for bitwise operators */
/* NB limited to sizeof(int); which is still 32bits on gcc under 64bit OS */

#include <stdio.h>
#include <limits.h>

enum bmode {B_AND,B_OR,B_XOR,B_COMP,B_LSHIFT,B_RSHIFT,NMODDES};
enum ARGS {PROGNAME, OP,OP_1,OP_2,N_ARGS};
const char* opname[] = {"&","|","^","~","<<",">>"};

/* for general interest...*/
typedef struct lex_token_u {
	int tokentype;
	union {
		int keyword;
		char* varname;
		int constant_ival;
		double constant_dval;
	} Token;
} UTOKEN;


void binprintf(int val);
const char* binfmt(int val);

int main(int argc,char**argv)
{
    unsigned int mode;
    int op1 = 0,op2 = 0,result = 0;
#if(0)
	printf("size of UTOKEN = %d\n",sizeof(UTOKEN));
#endif
    if(argc < N_ARGS){
        printf("insufficient arguments\n");
        printf("usage:  bitwise op operand_1 operand_2\n"
               "        Operations:\n"
               "        0  = AND\n"
               "        1 = OR\n"
               "        2 = XOR\n"
               "        3 = COMP (OP_2 ignored)\n"
               "        4 = Left Shift\n"
               "        5 = Right Shift\n"
               );
        return 0;
    }
	
	
    mode = atoi(argv[OP]);
    op1 = atoi(argv[OP_1]);
    op2 = atoi(argv[OP_2]);
    
    if(mode > B_RSHIFT){
        printf("usupported operation %u\n",mode);
        return 1;
    }

    switch(mode){
        case(B_AND):
            result = op1 & op2;
            break;
        case(B_OR):
            result = op1 | op2;
            break;
        case(B_XOR):
            result = op1 ^ op2;
            break;
        case(B_COMP):
            result = ~op1;
            break;
        case(B_LSHIFT):
            if(op2 <= 0){
                printf("Error: shift value must be positive\n");
                return 1;
            }
            result = op1 << op2;
            break;
        case(B_RSHIFT):
            if(op2 <= 0){
                printf("Error: shift value must be positive\n");
                return 1;
            }
            result = op1 >> op2;
            break;
        default:
            break;
    }
    /* using binfmt() very carefully: */
    printf("%d %s %d = %d\n",op1,opname[mode],op2,result);
    printf("0x%.8x %s 0x%.8x = 0x%.8x\n",op1,opname[mode],op2,result);
    printf("%s\n%s\n",binfmt(op1),opname[mode]);
    printf("%s\n=\n",binfmt(op2));
    printf("%s\n",binfmt(result));
    
    return 0;
}

/* two ways of printing binary: */

/* portable: no assumptions re size of int */

void binprintf(int val)
{
    unsigned int nbits = sizeof(int) * CHAR_BIT;
    unsigned int i;
    unsigned int mask = 1 << nbits-1; /* i.e. 0x80000000; */
    for(i=0;i < nbits;i++){
        if(i > 0 && i%8 == 0)
            printf(" ");
        printf("%d",(val & mask) ? 1 : 0);
        mask >>= 1;
    }
}

/* returns string, The static array decl ensures 
function is self-contained - but at the price that the 
internal array will be overwritten on later calls,
potentially giving false results unless copied externally,
or used VERY CAREFULLY */

/* Exercise: do this in a way that mimics sprintf */

const char* binfmt(int val)
{
    static char binword[/*36*/sizeof(int) * (CHAR_BIT+1)];
    unsigned int nbits = sizeof(int) * CHAR_BIT;
    unsigned int i;
    unsigned int mask = 1 << nbits-1; /* i.e. 0x80000000; */
    unsigned int chpos = 0;
    
    for(i=0;i < nbits;i++){
        if(i > 0 && i%8 == 0)
            binword[chpos++] = ' ';
        binword[chpos++] = (val&mask) ? '1' : '0';
        mask >>= 1;
    }
    binword[chpos] = '\0';
    return (const char*) binword;
}

    