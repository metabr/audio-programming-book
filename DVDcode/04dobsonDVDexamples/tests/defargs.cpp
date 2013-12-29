// simple demonstration of default arguments
#include <iostream>

unsigned int calc_samps (double length, unsigned int srate = 44100)
{
  return (unsigned int) (length * srate);
}
 


int main()
{ 
  unsigned int deflen = calc_samps(1024);
  unsigned int len48k = calc_samps(1024,48000);

  std::cout << "default length = " << deflen << std::endl;
  std::cout << "custom length  = " << len48k << std::endl;
  return 0;
}
