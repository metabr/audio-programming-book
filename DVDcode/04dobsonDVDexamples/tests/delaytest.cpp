// delaytest.cpp
#include <iostream>
#include <iomanip>  // for output formatting 
#include "../vdelaycpp/delay.h"

const int buflen(32);

int main()
{
	using namespace audiobook; 
   
    float inbuf[buflen];
    float outbuf[buflen];
    /* set up input data */
    for(int i = 0;i < buflen;i++)
      inbuf[i] = i+1;
    // setup and run the delay line
    unsigned int delaysamps = 10;
    delay mydelay;
    mydelay.init(delaysamps);
    
    for(int i = 0;i < buflen;i++)
        outbuf[i] = mydelay.tick(inbuf[i],0.0);
   
    std::cout << "contents of outbuf: \n";
    // output 8 fields per line, width = 4 digits, right-justified
    for(int i = 0; i < buflen;i++) {
        std::cout << std::right << std::setw(4) << outbuf[i];
        if((i+1) %8==0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
    /* test copy constructor by flushing data */
    std::cout << " contents of copied delay: \n";
    delay mydelay2(mydelay);
    for(int i = 0; i < buflen;i++) {
        std::cout << std::right << std::setw(4) 
              << mydelay2.tick(0.0,0.0);
        if((i+1) %8==0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
