#ifndef __TABLEh__
#define __TABLEh__

#include "OSCILIb.h"

class TABLE : private OSCILI // a class that implements only the table of an oscillator
{
  public:
    TABLE(long _tablen = TABLE_LEN, WAVESHAPE _shape = Sine) : OSCILI(_shape, 0, _tablen) { }
    float* getTable() const { return OSCILI::getTable(); }
    long getTableLength() const { return OSCILI::getTableLength(); }
    void setTableLength( long length) { OSCILI::setTableLength(length); }
    virtual void fillTable( WAVESHAPE s ) { OSCILI::fillTable(s); }
    virtual void fillTable( TABLEFUNC func, void *userData=0) { OSCILI::fillTable(func, userData); }
    virtual void fillTable( TABLEFUNC func, long len, void *userData=0) { 
        OSCILI::fillTable(func, len, userData); 
    }
};

class OSCILIextTABLE : public OSCILIb        // similar to OSCILIb, but with external table
{
  public:
    OSCILIextTABLE( TABLE& _table, double _sr = 44100, 
            float _amp = 1, float _freq = 440, 
            double initialPhase = 0, WAVESHAPE _shape = Custom) : OSCILIb(_shape, _sr)
    {
        delete [] table;     // we don't need embedded table so delete it
        setTable(_table);    // and update its pointer address to the external TABLE object
        if (shape != Custom) fillTable(_shape); // fill the table 
        block = Out.getBlock();
    }
    void setTable(TABLE& _table) {
        table =  _table.getTable(); // get the address of the table belonging to a TABLE object
        tablen = _table.getTableLength(); // get its length
    }
    ~OSCILIextTABLE() { table = 0; } // we need a destructor to avoid the table pointer is deleted
};

#endif