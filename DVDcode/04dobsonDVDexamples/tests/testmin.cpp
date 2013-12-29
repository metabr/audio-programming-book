// demonstrate supporting std::min and stream output for a class
#include <iostream>

// try removing various consts and look at the error messages!

// a useless class
class myobj  {
  public:
     myobj() : val_(0) {}
      myobj(int initval) : val_(initval) {}
    virtual ~myobj() {};
    // the essential overload
    bool  operator<(const myobj& rhs) const {
        // of course this relies on operator< being defined for this type!
        return val_ < rhs.val_;
    }
    // the essential "getter" function
    void printval(std::ostream& strm) const  { strm << val_; }
  private:
        // for demo purposes, this is a standard type supporting comparisons
        // but in principle could be some arbitrary type, also supported for <<  and < 
     int val_;
};
// this has to be outside the class because it is not an operator belonging to myobj,
// but is a new specialization of ostream, and we cannot change that directly!
inline std::ostream& operator <<(std::ostream& strm, const myobj& ob)  { ob.printval(strm); return strm;}

 
int main()
{
    // swap the values around ad lib...
   myobj ob1(40);
   myobj ob2(30);
   
   myobj res = std::min(ob1,ob2);
   std::cout << res << std::endl;
   
   return 0;
}
