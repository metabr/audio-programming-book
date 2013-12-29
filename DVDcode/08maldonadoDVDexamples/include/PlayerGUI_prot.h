#include <FL/Fl_File_Chooser.H>

void load(void *);
void play(void *); 
void stop(void *); 
void ff(void *);
void rew(void *);

enum VALUATOR { SLID, TXTSTART } ;

void setpos(void *, VALUATOR t);