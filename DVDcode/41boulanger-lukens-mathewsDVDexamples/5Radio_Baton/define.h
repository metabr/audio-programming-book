#ifndef DEFINE_H
#define DEFINE_H

// define.h
//ns and midi key codes
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
#define STICKLEVELS 25                    /*  set hit and set levels */
#define SYSEX 26
#define PITCHBEND 27
#define ENDSCORE 28
#define CHANNELPROGRAMCHANGE 29
#define MODE 31              /* y */


// nc.c and ns constants
#define VERSIONNO 100
#define MTICSPERCOMMA 8
#define SCOBUFSIZE 1024   //must be power of 2
#define SCOSIZE 4096      //must be 4*SCOBUFSIZE
#define SCOAND 0xfff      //must be SOCSIZE-1
#define SCOBUFAND 0x3ff   //must be SCOBUFSIZE-1
#define SMOOTHING 1 // SMOOTHING = 1,2,or 3 control change smoothing
#define NOCONTROL 16  //max number of patchchords
#define X 1
#define Y 2
#define Z 3
#define P 4
#define V 5
#define S 6
#define T 7
#define DATASIZE 10000
#define NUMBEROFEVENTS 1000
#define MIDIARRAYSIZE 262144
#define VOICES 50
#define NUMMACROS 27
#define MACSIZE 500
#define NOTRACKS 32
#define MIDIFILESIZE 1000

//bat.c and nc.c sysex communication codes
#define TEST 1
#define STOP 2
#define SCORE 3
#define PLAY 4
#define MMARK 5
#define NOBAT 6
#define SCAN  7
#define RUNS  8
#define STRING 9
#define TWOTIMBZ 10
#define TWOTIMBY 11
#define COMMAND 12
#define ADTEST 13
#define SDXYZ  14
#define STICKLEVELS 25
#define RUNS_STRING 26
#define WHACKTEST 28
#define RESET 30                       /* reset 186 to initial state */
#define STICKLEVELS1 32
#define UART 33
#define MORESCORE 34
#define SLOWSCORESEND 35
#define MEASURE 36
#define SYSX 37
#define NOBATOFF 38
#define QUIET 39
#define STARTSCORESEG 40
#define ENDOFSCORE 41
#define SCOBUFEMPTY 42
#define PAUSE 43
#define ENDUART 44
#define IMPROV 46
#define RECCALIB 47
#define SENDCALIB 48
#define ZEROCORRECTION1 49
#define ZEROCORRECTION2 50
#define RANGE 51
#define JAFFE_SCHLOSS 45
#define LOPEZ_LEZCANO 125    /* new protocol added by nando */

#define BADCALIB      1000    //lcd writing codes
//dt2814 port addresses
#define DTCONT    0x220
#define DTSTAT    0x220
#define DTDATA    0x221

//music quest pc midi port addresses and constants
#define MQSTAT    0x331
#define MQDATA    0x330
#define MQDATARDY 0x80
#define MQSENDRDY 0x40

//80c186 constants
#define tcucon 0xff12
#define t2cnt  0xff40
#define t2cmpa 0xff42
#define t2con 0xff46
#define port0latch 0xff56
#define s1baud 0xff70
#define s1cont 0xff74
#define s1stat 0xff76
#define s1tbuf 0xff7a
#define s1rbuf 0xff78
#define baud   0x803f
#define s1datardy 0x0040
#define adcont 0x0400
#define addata 0x0402

#endif
