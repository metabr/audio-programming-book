
// testcon.cpp
// demonstrates how to handle multiple allocs in constructor, 
// when one alloc throws an exception
// also demonstrates a rudimentrary class factory
// see "Design Patterns" for much more information on this sort of thing.
#include <iostream>

// the object we only want a set number of (e.g. 1)
class bigobj {};

// in order to demo a failed alloc, we make a factory to make a class!


// a custom trivial exception object
class bigobj_exception {};

// the factory for bigobjs - limit of bigobjs hard-wired in constructor
// as shown, only allows one object - a singleton. Will throw exception if a second is requested
// set maxobjs = 2 to have everything work!
class classfactory {
public:
    explicit classfactory(int maxobjs = 1) : n_created(0), limitobjs(maxobjs)  {}
    virtual ~classfactory() {}
    bigobj*  create_object()   {
        bigobj* obj(0);
        if(n_created == limitobjs)
            throw bigobj_exception();
        else {
            try {
                obj = new bigobj();
                n_created++;
            }
            catch(...){
                // send exception to outer scope
                throw;
            }
            return obj;
        }
    }
    int how_many() const { return n_created; }
private:
    int n_created;
    const int limitobjs;
};
    
// class that tried to create either one or two objects
// depending on whether commented-out line is activated
// the class can only create itself if it can get ~both~ internal objects created

class myclass {
public:
    myclass() : obj1(0), obj2(0)  
    {
	     try{    // two objects allocating memoery
             obj1 = factory.create_object();  // this succeeds
             obj2 = factory.create_object();  // this fails, throws exception
             // forcing us to cleap up the existing good object, then throwing exception
		 }
		 catch(...){
		    delete obj1;
  		    delete obj2;
		    throw;  // sends exception to caller
		 }
    }
    virtual ~myclass() { delete obj1; delete obj2; }
    int how_many() const { return factory.how_many(); }
           // etc
private:
    bigobj *obj1, *obj2;
    classfactory factory; // on the stack - default constructor
};

int main()
{ 
    // must use a pointer at toplevel scope (outside try block)
    myclass* p_class =0;
    
    try {
        p_class = new myclass;
    }
    catch(...){
        std::cout << "myclass object failed to create\n";
        return 1;
    }
    std::cout << "myclass object y created with " << p_class->how_many() << " objects" << std::endl;
    delete p_class;
    return 0;
}

/* EXERCISE!
   As shown, classfactory is hard-coded to permit creation of only one bigobj.
   What would you need to do, to parametrise this: to be able to set the limit to bigobjs
   at the level of main()? e.g. as an argument to myclass. 
   You cannot for example simply declare classfactory on the stack inside the 
   myclass definition, with an argument:

   classfactory factory(1); // error!   
*/
   
        