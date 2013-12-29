#ifndef __asignal__
#define __asignal__
#include <cstring>      // for memcpy()
typedef unsigned long U_LONG;

class ASIGNAL {
    double *block;
    static const U_LONG blockLen; // must be initialized globally before any object is created!
public:
    ASIGNAL() { block = new double[blockLen]; }
    ASIGNAL(const ASIGNAL& sig) { block = new double[blockLen]; for (U_LONG i=0; i < blockLen; i++) block[i] = sig.getBlock()[i];}
    ASIGNAL(const double x) { block = new double[blockLen]; for (U_LONG i=0; i < blockLen; i++) block[i] = x; }
    ASIGNAL(const double* _block) { block = new double[blockLen]; memcpy(block, _block, blockLen * sizeof(double)); }
    ~ASIGNAL() { delete[] block; }
    double* getBlock() const { return block; }
    static U_LONG getBlockLen() { return blockLen; }
    ASIGNAL& operator=(const ASIGNAL& s) { memcpy(block, s.getBlock(), blockLen * sizeof(double)); return *this; }
    ASIGNAL& operator=(const double* _block) { memcpy(block, _block, blockLen * sizeof(double)); return *this;  }
    ASIGNAL& operator=(const double x) { for (U_LONG i=0; i < blockLen; i++) block[i] = x; return *this;  }
    
    ASIGNAL& operator+=(const ASIGNAL& s) { for (U_LONG i=0; i < blockLen; i++) block[i] += s.getBlock()[i]; return *this; }
    ASIGNAL& operator-=(const ASIGNAL& s) { for (U_LONG i=0; i < blockLen; i++) block[i] -= s.getBlock()[i]; return *this; }
    ASIGNAL& operator*=(const ASIGNAL& s) { for (U_LONG i=0; i < blockLen; i++) block[i] *= s.getBlock()[i]; return *this; }
    ASIGNAL& operator/=(const ASIGNAL& s) { for (U_LONG i=0; i < blockLen; i++) block[i] /= s.getBlock()[i]; return *this; }

    ASIGNAL& operator+=(const double x) { for (U_LONG i=0; i < blockLen; i++) block[i] += x; return *this; }
    ASIGNAL& operator-=(const double x) { for (U_LONG i=0; i < blockLen; i++) block[i] -= x; return *this; }
    ASIGNAL& operator*=(const double x) { for (U_LONG i=0; i < blockLen; i++) block[i] *= x; return *this; }
    ASIGNAL& operator/=(const double x) { for (U_LONG i=0; i < blockLen; i++) block[i] /= x; return *this; }
};

inline  ASIGNAL operator+ (const ASIGNAL& a, const ASIGNAL& b) { return ASIGNAL(a) += b; }
inline  ASIGNAL operator- (const ASIGNAL& a, const ASIGNAL& b) { return ASIGNAL(a) -= b; }
inline  ASIGNAL operator* (const ASIGNAL& a, const ASIGNAL& b) { return ASIGNAL(a) *= b; }
inline  ASIGNAL operator/ (const ASIGNAL& a, const ASIGNAL& b) { return ASIGNAL(a) /= b; }

inline  ASIGNAL operator+ (const ASIGNAL& asig, const double x) { return ASIGNAL(asig) += x; }
inline  ASIGNAL operator- (const ASIGNAL& asig, const double x) { return ASIGNAL(asig) -= x; }
inline  ASIGNAL operator* (const ASIGNAL& asig, const double x) { return ASIGNAL(asig) *= x; }
inline  ASIGNAL operator/ (const ASIGNAL& asig, const double x) { return ASIGNAL(asig) /= x; }

inline  ASIGNAL operator+ (const double x, const ASIGNAL& asig) { return asig + x; }
inline  ASIGNAL operator- (const double x, const ASIGNAL& asig) { return asig - x; }
inline  ASIGNAL operator* (const double x, const ASIGNAL& asig) { return asig * x; }
inline  ASIGNAL operator/ (const double x, const ASIGNAL& asig) { return asig / x; }

inline void asigStereoOut(float* out, const ASIGNAL& sig) { 
    U_LONG n = sig.getBlockLen();
    float  *o = out;
    double *b = sig.getBlock();
    do {
        *o++ = (float) *b;
        *o++ = (float) *b++;
    } while(--n);
}

inline void asigStereoOut(float* out, const ASIGNAL& sigLeft, const ASIGNAL& sigRight) {
    U_LONG n = sigLeft.getBlockLen();
    float  *o = out;
    double *left = sigLeft.getBlock();
    double *right = sigLeft.getBlock();
    do {
        *o++ = (float) *left++;
        *o++ = (float) *right++;
    } while(--n);
}

inline void asigStereoIn(float* in, ASIGNAL& sigLeft, ASIGNAL& sigRight) {
    U_LONG n = sigLeft.getBlockLen();
    float  *i = in;
    double *left = sigLeft.getBlock();
    double *right = sigLeft.getBlock();
    do {
        *left++ = (double) *i++;
        *right++ = (double) *i++;
    } while(--n);
}

#endif