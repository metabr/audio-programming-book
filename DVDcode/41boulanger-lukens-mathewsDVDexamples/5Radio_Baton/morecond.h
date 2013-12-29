#ifndef MORECOND_H
#define MORECOND_H


#define DEFAULTTRIGGERLEVEL 3001
#define BUFFER_SIZE 4096

#define INPUT_BUFFER_SIZE 100
#define OUTPUT_BUFFER_SIZE 0
#define DRIVER_INFO NULL
#define TIME_PROC ((long (*)(void *)) Pt_Time)
#define TIME_INFO NULL
#define TIME_START Pt_Start(1, 0, 0) /* timer started w/millisecond accuracy */

#define STRING_MAX 80 /* used for console input */


//global variables
long t_time;
long starttime;
int midiinint;
int exitflag;
char **argv;
int argc;

//cond2 variables
unsigned char midibyte0,midibyte1,midibyte2;
int midistate=0;
int modee,play,triggerFlag,calibrationflag;
long silencetime;
int playmode=1;
short sysexmessage,midich1,maxscore,minscore,sendingscore,timecount;
unsigned char midi_port;
unsigned char midififo[256];
unsigned int midiip,midiop,midiipsav;
short midimessages,sysexcase,scorechars,charcount;
unsigned int scoreip,scoreop;
long controlchangetime,lastwhacktime;
int startmeasure;
int scan,discard;
int measure;
int readscore;
FILE *fp2;
short buf[17];
int b14minusdown;
int b14minusup;
int b15minusdown;
int b15minusup;
float tempo,temposave;
int pt1, pt2, pt3, pt4;           // current dial (pot) positions
int xt1, yt1, zt1, xt2, yt2, zt2; // current stick positions
int wh1, wh2;                     // whack strength of trigger
int whack1, whack2;               // whack detected boolean
int xw1, yw1, xw2, yw2;           // x, y values at whack time
int hit1, set1, hit2, set2;       // hit and set levels of sticks
long  trigtime1, trigtime2;         // last trigger time of sticks
int anynoteon;

//varables for condcordless baton
int buttons,buttoncommand,feet,feetcommand;
int timbre[100]={0,0,9,11,14,47,11,14,46,47,0,46,47,9,0,11,14,46,47,14,11,0,-1};
int timbrenumber;
float xc1,yc1,zc1,zc1p,xc2,yc2,zc2;
float triggerlevel,resetlevel,sustainlevel;
float  array[9];
int intarray[9];
int triggerreset1=1,triggerreset2=1;
int printbatonflag=0;
int printADflag=0;
long nextprintbatontime;

struct timeval          tb, ta, td;

//portmidi variables
int deviceIn, deviceOut;
PmEvent bufferIn[1], bufferOut[1];

long latency = 0;


#endif // MORECOND_H
