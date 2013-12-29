#include <iostream>

// this is supposed to generate a Fibonnacci sum (where fib(n) = fib(n-1) + fib(n-2) )
// but according to a  C++ Standard committee Defect Report, the result are undefined
// as the order of instantiations of the templates is undefined.
// see: http://std.dkuug.dk/JTC1/SC22/WG21/docs/cwg_defects.html
// an exotic example of using templates to do iteration
template<int n>
  struct Fib { static unsigned long Value; };

  template <>
  unsigned long Fib<0>::Value = 0;

  template <>
  unsigned long Fib<1>::Value = 1;

  template<int n>
  unsigned long Fib<n>::Value = Fib<n-1>::Value + Fib<n-2>::Value;  // possible problem here 
  
  // NB:  BIG disadvantage: this HAS to be an explicit const expression (i.e. a specific constant number)
 // in effect, like declaring an array. We cannot supply it as a parameter!
  unsigned long f ()
  {
    return Fib<46>::Value;
  }
  
  
  int main()
  {
      unsigned long val =  f(); 
      std::cout << "val = " << val << std::endl;
      return 0;
  }