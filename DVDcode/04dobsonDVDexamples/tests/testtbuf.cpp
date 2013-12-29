// testtbuf.cpp: demonstrate use of tbuffer template
#include <iostream>
#include "tbuffer.h"
extern "C"{
#include "breakpoints.h"
}

using namespace audiobook;

int main()
{
    unsigned long size = 44100;
    tbuffer<double> dbuf;
    dbuf.init(size);
    std::cout << "buffer size = " << dbuf.size() << std::endl;
    for(int i= 0;i < 32;i++) {
        dbuf[i] = (double)(i*1.5);
        std::cout <<  dbuf[i] << std::endl;
    }
    std::cout << std::endl;
    
    tbuffer<BREAKPOINT> brkbuf;
    unsigned int npoints(10);
    brkbuf.init(npoints);
    std::cout << "breakpoint bufer size = " << npoints << std::endl;
    for(unsigned int i = 0; i < npoints;i++){
        brkbuf[i].time = i;
        brkbuf[i].value = i*1.5;
        std::cout << brkbuf[i].time << ":" << brkbuf[i].value << std::endl; 
    }
    
    return 0;
}