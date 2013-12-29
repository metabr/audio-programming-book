#ifndef _TBUFFER_H_
#define _TBUFFER_H_
// simple template wrapper class for an array
namespace audiobook {
    template<class T> class tbuffer {
    public:
        tbuffer() : buf_(0), length_(0)     {}
        tbuffer(const tbuffer& rhs) {
            buf_ = new T[rhs.length_];
            for(unsigned long i = 0; i < rhs.length_; i++)
                buf_[i] = rhs.buf_[i];
            length_ = rhs.length_;
        }
        virtual ~tbuffer()      { delete [] buf_; };
        void init(unsigned long size){
            delete [] buf_;
            buf_ = new T[size];
            length_ = size;
        }
        tbuffer& operator=(const tbuffer& rhs){
            if(this != &rhs){
                delete [] buf_;
                buf_ = new T[rhs.length_];
                length_ = rhs.length_;
                for(unsigned long i = 0; i < length_; i++)
                    buf_[i] = rhs.buf_[i];
            }
            return *this;
        }
        unsigned long size()      const { return length_;}
        // return reference: support array read/write
        T& operator[](unsigned long pos) {return buf_[pos];} 
        const T& operator[](unsigned long pos) const 
              {return buf_[pos];}
        void clear() { delete [] buf_; length_ = 0; buf_ = 0;}
    private:
        T* buf_;
        unsigned long length_;
    };
}  // namespace audiobook
#endif
