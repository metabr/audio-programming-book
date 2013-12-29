// dyncast.cpp: demo of dynamic_cast and typeid
// compiler must support RTTI!
#include <iostream>
// need this to use typeid
#include <typeinfo>

// base class
class instrument {
public:
    instrument() {}
    // avoid compiler warning: class with virtual funcs should have virtual destructor
    virtual ~instrument() {}     
    virtual void play() {std::cout << "play!\n";}
    virtual void hit(int beater) { std::cout << "dented! " << beater << std::endl;}
    void packup() { std::cout << "in the case!\n";}
};

// derived class
class drum : public instrument {
public:
    drum() {}
    virtual ~drum() {}
     virtual void play() {std::cout << "shake!\n";}
     virtual void hit(int beater) {std::cout << "hit! " << beater << std::endl; }
     void change_beater(int beater) { std::cout << "new beater ! " << beater << std::endl;}
};

// why not make a third instrument?
// perhaps one with a method "blow"?

// input could be either an instrument or any derived object
bool isdrum(const instrument& instr, int testit)
{
    bool result = false;
    // try out typeid
    std::cout << "type is: " << typeid(instr).name() << std::endl;
    try{
        drum& tmpdrum = dynamic_cast<drum&>(const_cast<instrument&>(instr));
        tmpdrum.play();
        tmpdrum.hit(testit);
        tmpdrum.change_beater(100);
        result = true;
    }
    catch(const std::bad_cast&){
        const_cast<instrument&>(instr).play();
        const_cast<instrument&>(instr).hit(testit);
        const_cast<instrument&>(instr).packup();
    }    
    return result;
}

// experiment with declarations ad lib
// we can pass both types to isdrum()

int main()
{
    drum instr1;
    instrument instr2;
    
    if(isdrum(instr1,20)) {
        std::cout << "that was a drum\n";
    }
    else
        std::cout << " not a drum\n";
    if(isdrum(instr2,30))
        std::cout << "that was a drum\n";
    else
        std::cout << " not a drum\n";
    
    return 0;
}

