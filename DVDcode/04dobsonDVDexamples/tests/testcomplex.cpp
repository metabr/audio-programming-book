// test complex 
#include <iostream>
#include <complex>

int main()
{
    std::complex<double> c1(0.5,0.4), c2(c1);    
    for(int i = 0;i < 10; i++) {
       c1 *= c2; // a spiral!
       std::cout << c1 << std::endl;
    }
    // check this gives the same result:
    std::cout << "pow(c2,11.0) = " << std::pow(c2,11.0) << std::endl;
    // test Pythagoras!
    std::complex<double> cs3(3.0,4.0);
    std::cout << "Hypotenuse = " << std::abs(cs3) << std::endl;
    // and now for something ... 
    std::complex<std::string> cs1("John ","Paul ");
    cs1 += std::complex<std::string>("George", "Ringo");
    std::cout << "complex beatles: " << cs1 << std::endl;
   
   return 0;
}  