// header file for text output: needed in all test programs
#include <iostream>
// c++ version of <math.h>
#include <cmath>

// a global contsant. Static variables (well, static anythings) are created 
// by the compiler (at runtime)  ~before~ main is called.
static const double defsemi = std::pow(2.0,1.0/12.0);  // method 1

class etscale
{
public:
         // constructors use initialiser list - we can use function calls in them too
    etscale() : default_semi(defsemi), semitone_(default_semi)          {}
    etscale(double interval,unsigned int div) :  default_semi(defsemi),
       // remove the final ) below to see lots of not very helpful error messages!
                                                semitone_(std::pow(interval,1.0/div)) {} 			
    double interval(double nSemis)  const { return std::pow(semitone_,nSemis);  }
private:
    const double default_semi;
    /* 
     OR method 2:
    static const double default_semi  = 1.0594630943592953;
    */
    double semitone_;	
};

//  use example: 

int main()
{
    etscale tone12(2.0,12);
    std::cout.precision(20);  // set numerical precision for output
    std::cout << "default semitone = " << tone12.interval(1) << std::endl;

    double fifth = tone12.interval(7.0);
    std::cout << "fifth = " << fifth << std::endl;
    etscale weirdscale(1.5,19);
    fifth = weirdscale.interval(7.0);
    std::cout << "weird fifth = " << fifth << std::endl;
    return 0;
}

