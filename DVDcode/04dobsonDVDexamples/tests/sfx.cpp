//sfx.cpp  basic custom exception
#include <iostream>
/* for std exception classes */
#include <stdexcept>

/* our custom exception class */
class sfile_exception
{
 public:
    sfile_exception(const std::string& errmsg) : msg(errmsg) {}
    const char* message() const throw() {return msg.c_str();}
    ~sfile_exception() throw() {}
 private:
	std::string msg;
};


int gen_error()
{
	std::cout << "busy...\n";
	throw sfile_exception("there was an error!\n");
	/* or use std exception class such as: 
	throw std::runtime_error("there was a runtime error\n");
	*/
}

int main()
{

	try{
	   gen_error();
	}
	catch(const sfile_exception& x)
	{
		std::cout << x.message() << std::endl;
	}
	catch(const std::runtime_error& x)
	{
		std::cout << x.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "some other exception!\n";
	}
	return 0;
}
	
