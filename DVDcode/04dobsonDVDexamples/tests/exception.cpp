/* exception.cpp */
#include <cstdio>
#include <iostream>

// useless skeleton class wrapping FILE 
class datafile
{
public:
    
	datafile(const char* filename);
	~datafile()       { if(fp_) fclose(fp_); }
    // a custom (empty) exception object, belongs to datafile
    class file_exception  {}; 
private:
	FILE* fp_;
};

datafile::datafile(const char* filename)     
{
	fp_ = fopen(filename,"r");
	if(fp_ == 0)
		throw file_exception();
}

// tries to open a file which does not actuially exist.
int main()
{		
	datafile* fpData = 0;
	
	try{
		fpData = new datafile("data.dat");
        std::cout << "file opened OK\n";
	}
	catch(const datafile::file_exception& fe){
		printf("failed to open data file\n");
		// do anything else we might need...
	}
	
	size_t big(0x7fffffff);
	float* p_buffer = 0;
	try {
		p_buffer = new float[big];		
        std::cout << "got memory OK\n";
	}
	catch(std::bad_alloc& ex){
        std::cout << "no memory!\n";
		// maybe release some other memory and try again...
	}
	// did we get anything?  clean it all up anyway
	delete  [] p_buffer;
	delete fpData;

	return 0;
}
