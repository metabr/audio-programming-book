// test istringstream to read text into a double; validate it as a number
// this replaces C strtod()
// NB we can ask the stream to use exceptions!
#include <iostream>
#include <sstream>

int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cout << "usage: teststream arg\n";
        return 0;
    }
    double val;
    // load the arg from user
    std::istringstream strm(argv[1]);
    
    try {
        // either turn on exceptions....
        //   strm.exceptions(std::ios::failbit | std::ios::badbit);         
        strm >> val;
        // OR use the istringstream fail() method (don't need try-catch block )
        if(strm.fail()) {
            std::cout<< "the fail() method worked!\n";
        }
        std::cout << "got val  = " << val << std::endl;
    }
    catch(...){
        std::cout << "that was not a number: " << argv[1] << std::endl;        
    }
    strm.clear();  // clear the error flags in case we want to use strm again
    std::cout << "OK!\n";
    return 0;
}
               
        