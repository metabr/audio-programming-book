#include <stdlib.h>
#include <stdio.h>

#include "midilib.h"
#include "midilib-i.h"

Note *score = NULL;

/* Internal function to free a score structure */
static void destroy_score(Note* sc)
{
    while (sc!=NULL) {
      Note* nxt = sc->next;
      free(sc);
      sc = nxt;
    }
}

/* Public interface function to start a new score */
/* Should it check to see if score was played or saved? */
int new_score(void)
{
    if (score!=NULL) destroy_score(score);
    score = NULL;
    return SUCCESS;
}

/* Create a new note structure and populate */
static Note *create_note(int pitch, int volume, int instr,
                         int when, int length)
{
    Note *newnote = (Note *)malloc(sizeof(Note));
    if (newnote==NULL) {
      /* Malloc failed so return error */
      return NULL;
    }
    newnote->pitch = pitch;
    newnote->volume = volume;
    newnote->instr = instr;
    newnote->when = when;
    newnote->length = length;
    newnote->next = NULL;
    return newnote;
}

int add_note(int pitch, int volume, int instr,
             int when, int length)
{
    Note *newnote = create_note(pitch, volume, instr,
                               when, length);
    Note *note = score;
    Note *previous = NULL;
    if (newnote==NULL) return ERR_MEMORY;

    while (1) {
      if (note==NULL) {
        if (previous==NULL) {
          score = newnote;
        }
        else {
          newnote->next = note;
          previous->next = newnote;
        }
        break;
      }
      if (when<=note->when) {
        /* Note comes before current note */
        newnote->next = note;
        if (previous==NULL) {
          score = newnote;
        }
        else {
          newnote->next = note;
          previous->next = newnote;
        }
        break;
      }
      /* Comes later */
      previous = note;
      note = note->next;
    }
    if (volume!=0)
      return add_note(pitch, 0, instr, when+length, 0);
    else return SUCCESS;
}


/* Support code to write MIDI files */

static int trk_cnt = 0;

static void print_int_be(FILE* f, int n)
{
    putc((n>>24)&0xff,f);
    putc((n>>16)&0xff,f);
    putc((n>>8)&0xff,f);
    putc(n&0xff,f);
    trk_cnt += 4;
}

static void print_short_be(FILE* f, short n)
{
    putc((n>>8)&0xff,f);
    putc(n&0xff,f);
    trk_cnt += 2;
}

static void print_variable(FILE *f, int n)
{
    if (n==0) {                 /* Treat zero as special case */
      putc(0, f);
      trk_cnt += 1;
    }
    else {                      /* need up to 4 bytes */
      int i = 3;
      char buffer[4];
      buffer[0] = n&0x7f;
      buffer[1] = (n>>7)&0x7f;
      buffer[2] = (n>>14)&0x7f;
      buffer[3] = (n>>21)&0x7f;
      while (buffer[i]==0) i--; /* Find first non-zero 7bit byte */
      do {
        if (i!=0) putc(buffer[i]|0x80, f);
        else putc(buffer[i], f); /* Last byte does not have 0x80 */
        trk_cnt++;
        i--;
      } while (i>=0);
    }
}

static FILE* open_MIDI_file(char *name)
{
    FILE *fout = fopen(name, "wb");
    if (fout==NULL) return NULL;
    /* Write Header */
    print_int_be(fout, MThd);         /* Type */
    print_int_be(fout, 6);            /* length = 6 */
    print_short_be(fout, 0);          /* format single track */
    print_short_be(fout, 1);          /* ntrks = 1 */
    print_short_be(fout, 0xe728);     /* division in milliseconds */
    /* Track Header */
    print_int_be(fout, MTrk);
    print_int_be(fout, 000);          /* length to be overwritten */
    trk_cnt = 0;
    return fout;
}

typedef struct {
  int instr;
  int count;
} CHAN;
static CHAN chans[16] = { {0,0},{0,0},{0,0},{0,0},
                          {0,0},{0,0},{0,0},{0,0},
                          {0,0},{0,0},{0,0},{0,0},
                          {0,0},{0,0},{0,0},{0,0}};

static int map_instr_chan(FILE *fout, int instr, int t)
{
    int i;
    int j=15;
    int low = 0x7fff;
    for (i=0; i<16; i++) {
      if (chans[i].instr==instr) {
        chans[i].count++;
        return i;
      }
      if (chans[i].count<low) {
        low = chans[i].count;
        j = i;
      }
    }
    /* Need program change */
    print_variable(fout, t);
    putc(PROGCHN|j, fout);
    putc(instr-1, fout);
    chans[j].instr = instr;
    chans[j].count = 1;
    return j;
}


static void write_score(FILE *fout)
{
    Note *note = score;
    int time = 0;
    while (note!=NULL) {
      /* Night need a program change */
      int chan = map_instr_chan(fout, note->instr,note->when - time);
      /* First the time delay */
      print_variable(fout, note->when - time);
      time = note->when;
      putc(NOTEON|chan, fout);
      putc(note->pitch, fout);
      putc(note->volume, fout);
      trk_cnt += 3;
      note = note->next;
    }
    /* Need an end-of-track meta event after time zero */
    putc(0, fout); putc(0xff, fout); putc(0x2f, fout); putc(0x00, fout);
    trk_cnt += 4;
}

static int close_MIDI_file(FILE *fout)
{
    int err;
    err  = fseek(fout, 18L, SEEK_SET); /* rewind to length field */
    print_int_be(fout, trk_cnt);
    return fclose(fout);
}


int save_score(char *name)
{
    FILE *f = open_MIDI_file(name);
    if (f==NULL) return ERR_FAIL_SAVE;
    write_score(f);
    return close_MIDI_file(f);
}
