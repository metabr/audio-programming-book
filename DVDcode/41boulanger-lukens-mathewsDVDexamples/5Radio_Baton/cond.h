#ifndef COND_H
#define COND_H

//begin
// cond.h for dos newbat
#define NOCONTROL 16/* max number of patchchords for control changes */
#define CHORDSIZE 15                   /* max no of notes in a chord */
#define VOICES 50                            /* maximum no of voices */
#define SCORERECSIZE 30            /* max no bytes in a score record */
#define NOTE 1
#define TRIG 2              /* / */
#define REST 4              /* r */
#define PROGRAMCHANGE 5     /* t */
#define NOOP 6
#define SETCHANNEL 7        /* h */
#define TEMPO 8             /* v or V */
#define UPPEDAL 9           //P
#define PEDAL 10            // p
#define CONTTEMPO 11        /* w */
#define MARK 12             /* m */
#define KEYVELOCITY 13      /* k */
#define BATON2 14           /* T */
#define DOTEQUALS 15        /* j */
#define INIT 19             /* I */
#define FTEMP 20            /* W */
#define CONTCONTROL 21      /* q */
#define MNOTEON 22
#define MNOTEOFF 23
#define NOACTION 24
#define STICKLEVELS 25
#define SYSEX 26
#define PITCHBEND 27
#define ENDSCORE 28
#define CHANNELPROGRAMCHANGE 29
#define MODE 31
#define MEASURE 36

// nc.c and ns constants
//#define SMOOTHING 0.5 // 0.0==infinite smoothing, 1.0==no smoothing
#define X 1
#define Y 2
#define Z 3
#define P 4
#define V 5
#define S 6

//cond2simple.c
#define NOTEON 9
#define CONTROLCHANGE 0xb
#define PROGRAMCHG 0xc

#define PITCHWHEEL 0xe
short a[28],abatset,arg1,arg2;
long beatsum;
long beat_time;
short b[28],baton2,bufchan;
unsigned char bufp;
short b1,b2;
unsigned char calf[2048];
short chan;
short channel[VOICES];
short chord[VOICES*CHORDSIZE];
short chordpi[VOICES],chordp[VOICES];
short controler;
short cont4,contp,ctempo,contpatch;
short contch[NOCONTROL],contno[NOCONTROL];/* arrays defining patchcords */
short controlerfn[NOCONTROL],controlerfnv[NOCONTROL];
int xform[NOCONTROL];
float xforma[NOCONTROL],xformb[NOCONTROL];
float tx0,tx1,ty0,ty1;
short sustped[16];
long  contime;
short dataa,dist;
short dotchange,dotro,dotrn;
short diff[11];
short dx,dr0,drumset1,drumset2;
short dynamic[VOICES],dyntype[VOICES];
long dur;
short endscoreflag;
//extern short hit1,hit2;short hungnote,hungnoteno;
long ibeat,itim;
short interval;
short kk;
short keych,keyno,keyvel;
long keytime;
short mark,midich,midichar,minus14,minus15,mm;
unsigned char midibuf[256],midirbuf[256];
unsigned char midipi,midipo,midirpi,midirpo;
short num,nextkeyno,noteon,nextbeat,nobat,needscore;
short notenumb1,notenumb2,noteon1,noteon2;
long nexttime,nextnotetime;
short plus14,plus15;
short potvalue,pot[5];
long pt_time,pt_beat;
short prog;
short recordmark,retoff,retseg,restpoint,recordcnt,recordlength;
long samptime;
short samp,scanmark,scancount,sustain;
short scorestate,scoreadp,scorkd[SCORERECSIZE],scorkdend;
//extern short set1,set2;short stick,st0,st1,st2,set12p,sticklevelflag;
short sum,suminc,range;
short t[28],temposet,thru;
long threshtime;

char scoref[100]={"score"};
short newdotp,scoresok;
int value;
char midif[100],scorepf[100]={"score.p"};
short vtemp;


#endif
