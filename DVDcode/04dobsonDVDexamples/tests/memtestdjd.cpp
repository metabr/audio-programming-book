/* modified verison of dj delorie memory tester to find maximum alloc */
/* see http://www.cygwin.com/cygwin-ug-net/setup-maxmem.html */
/* and http://www.delorie.com/users/dj */

/* no makefile needed: type "make memtestdjd" to build */
#include <iostream>
#include <new>

/*****
 Try removing the try/catch block to see the program 
 exit in response to bad_alloc exception! 
 Some systems will report diagnostic information from the memory mamager
 when this is run. 
 
 There are two related questions: 
 1: the largest single block that is able to be allocated
 2: max alloc overall.
 A single large block request can fail, but multiple smaller 
 ones of the same or larger size overall might succeed.
 *******/

bool global_memory_failure(false);

void my_new_handler()
{
    std::cout << "\n***** new failed: handling the exception!" << std::endl;
    // set our arbitary global var
    global_memory_failure = true;
    throw std::bad_alloc();
}


int main()
{
    unsigned long bit(0x80000000), sum(0);
    unsigned long goodnews(0);
    char *x;
    
    std::set_new_handler(my_new_handler);
    
    while (bit > 4096) 
    {
        try{
            x = new char[bit]; // of course we lose access to each x as we allocate over it!
            std::cout << "+";
            goodnews++;
            sum += bit;  // only here if new succeeds        
            bit >>= 1;
        }
        catch(...){
            std::cout << "exception caught" << std::endl;;
            bit >>= 1;  // try again with smaller size
            if(global_memory_failure){
                std::cout << "failure size = " << bit << std::endl;
                global_memory_failure = false;
                continue;
            }
        }
    }
    std::cout << "There were " << goodnews << " sucessful allocations." << std::endl;
    std::cout << std::hex << sum << " bytes (" << sum/1024.0/1024.0 << "Mb)\n";
    std::cout << "bit = " << std::hex <<  bit << std::endl;
    return 0;
}
