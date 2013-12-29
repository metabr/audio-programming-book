// constcast.cpp: demo of const_cast<>()
#include <iostream>
#include <string>
#include <stdio.h> // yes we can have both!

class sfile{
public:
    explicit sfile(std::string name) : name_(name), player_("none") {}
    const std::string& what() const { return name_;}
    void set_player(std::string name) {player_ = name;}
    const char* player() const { return player_.c_str(); }
private:
    std::string name_;
    std::string player_;
};

void play(const sfile& pfile)
{  
    // we actually need to update this object
    const_cast<sfile&>(pfile).set_player("Radio 3");
    std::cout << "playing "   << pfile.what() << std::endl;    
}

// some old function in some old C library
extern "C" void show_player(char* name){
    printf("Another cool broadcast from %s!\n",name);
}

int main()
{
    sfile sting("jingle.wav"); 
    play(sting);  // requires conversion inside play()
    // show_player(sting.player());  // error: invalid conversion
    show_player(const_cast<char*>(sting.player()));
    return 0;
}
