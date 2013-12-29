// runerr.cpp
#include <iostream>
#include <stdexcept>
// try removing this header and see what happens!

int main()
{
     try{
        throw std::runtime_error("the string broke!");
    }
    catch(std::runtime_error& ex){
        std::cout << "error: " << ex.what() << std::endl;
    }
    return 0;
}

    