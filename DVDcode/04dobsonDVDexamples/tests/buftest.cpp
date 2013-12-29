// buftest.cpp
// exercises the buffer class
// two ways of specifying a namespace
// change the constant in buf.init() (and the for loop!) ad lib, 
//   but with care: the output could get long!
#include <iostream>
#include "../vdelaycpp/vdelay.h"

// use the namespace qualifier in the arg definition
void printbuf(const audiobook::buffer& buf, int len)
{
    for(int i = 0;i < len;i++) {       
        std::cout << "buf[" << i << "] = " << buf[i] << std::endl;
    }    
}

int main()
{  
    using namespace audiobook;

    buffer buf;
    buf.init(10UL);
      
    for(int i = 0;i < 10;i++){
        buf[i] = i*2;
        std::cout << "buf[" << i << "] = " << buf[i] << std::endl;
    }
    // and again, using a function
    printbuf(buf,10);
    return 0;
}