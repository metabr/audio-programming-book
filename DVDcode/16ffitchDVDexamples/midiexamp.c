#include <stdlib.h>
#include <stdio.h>
#include "midilib.h"

#define W_C (100)
#define W_A (200)
#define W_G (150)
#define W_E (256)

int weightC[8];
int weightA[8];
int weightG[8];
int weightE[8];

#define FLUTE    (74)
#define CLARINET (72)
#define OBOE     (69)
#define HORN     (61)
#define BASSOON  (71)

#define BEAT (700)

/* #define FLUTE    (0) */
/* #define CLARINET (0) */
/* #define OBOE     (0) */
/* #define HORN     (0) */
/* #define BASSOON  (0) */

void initialise(void)
{
   int i;
   for (i=0; i<8; i++) {
      weightC[i] = W_C;
      weightA[i] = W_A;
      weightG[i] = W_G;
      weightE[i] = W_E;
   }
}

int chose(int weights[8])
{
   int i;
   int total = 0;
   int random_value;
   for (i=0; i<8; i++) total += weights[i];
   random_value =  (int)(random()%total);
#ifdef DEBUG
   printf("total,random_value = %d,%d\t", total, random_value);
#endif
   for (i=0; i<7; i++) {
      random_value -= weights[i];
      if (random_value<0) {
#ifdef DEBUG
        printf("->%d\n", i);
#endif
        return i;
      }
   }
#ifdef DEBUG
   printf("->7\n");
#endif
   return 7;
}


int main(int argc, char **argv)
{
   int i;
   int target = 3;
   int time = 0;
   int pitch[] = {60, 62, 64, 65, 67, 69, 71, 72};
   int b1, b2, b3, b4;
   initialise();		/* Initialise tables */
   new_score();			/* Initialise score */
   while (target != 0) {        /* Loop until second CAGE */
     int amp = random() % 10;   /* Amplitude variation/bar */
      b1 = chose(weightC);
      add_note(pitch[b1], amp+200, FLUTE, time, 4*BEAT);
      time += BEAT;
      b2 = chose(weightA);
      add_note(pitch[b2], amp+180, CLARINET, time, 2*BEAT);
      time += BEAT;
      b3 = chose(weightG);
      add_note(pitch[b3], amp+190, OBOE, time, 2*BEAT);
      time += BEAT;
      b4 = chose(weightE);
      add_note(pitch[b4], amp+175, HORN, time, BEAT);
      time += BEAT;
#if DEBUG
      printf("%.4d: %d %d %d %d\n", time, b1, b2, b3, b4);
#endif
      /* Adjust weights */
      for (i=0; i<8; i++)
        if (i!=0 && i!=7) if (weightC[i]>0) weightC[i]--;
      for (i=0; i<8; i++) if (i!=6) if (weightA[i]>0) weightA[i]--;
      for (i=0; i<8; i++) if (i!=5) if (weightG[i]>0) weightG[i]--;
      for (i=0; i<8; i++) if (i!=3) if (weightE[i]>0) weightE[i]--;
      /* Did we get CAGE?? */
      if ((b1==0 || b1==7) && b2==6 && b3==5 && b4==3) {
        target--;
#if DEBUG
        printf("Target\n");
#endif
      }
   }
   for (i=0; i<2; i++) {
     add_note(pitch[b1], 200, FLUTE, time, 4*BEAT);
     time += BEAT;
     add_note(pitch[b2], 180, CLARINET, time, 2*BEAT);
     time += BEAT;
     add_note(pitch[b3], 190, OBOE, time, 2*BEAT);
     time += BEAT;
     add_note(pitch[b4], 175, HORN, time, BEAT);
     time += BEAT;
   }
   add_note(12+pitch[b1], 100, FLUTE, time, 4*BEAT);
   add_note(pitch[b1], 100, CLARINET, time, 4*BEAT);
   add_note(pitch[b3], 100, OBOE, time, 4*BEAT);
   add_note(pitch[b4], 100, HORN, time, 4*BEAT);
   save_score("cage.mid");
   printf("Piece was %d beats long\n", 4+time/BEAT);
}
