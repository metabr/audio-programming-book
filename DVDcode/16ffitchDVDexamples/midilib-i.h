typedef struct note {
    int          pitch;
    int          volume;
    int          instr;
    int          when;
    int          length;
    struct note* next;
} Note;

extern Note* score;

#define MThd     (0x4d546864)
struct Hdr_Chk {
  int     type;     /* Must be 'MThd', 0x4d546864 */
  int     length;   /* length of header chunk - two words == 6 */
  short   format;   /* Singletrk, multitrk or multi song */
  short   ntrks;    /* number of tracks */
  short   division; /* Time units in beats */
};

#define MTrk     (0x4d54726b)
struct Trk_Hdr {
  int     type;     /* Must be 'MTrk', 0x4d54726b */
  int     length;   /* no. of bytes in the track chunk following */
};

#define NOTEON  (0x90)
struct NoteOn {
    char    eventch;
    char    pitch;
    char    velocity;
};
#define PROGCHN  (0xc0)

