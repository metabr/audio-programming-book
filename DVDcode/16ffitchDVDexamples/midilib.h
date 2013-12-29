/* ******************************************************************** */
/* ********  Public Header file for MIDILib (John ffitch,2003) ******** */
/* ******************************************************************** */

int new_score(void);
int add_note(int /*pitch*/, int /*volume*/, int /*instr*/,
             int /*when*/, int /*length*/);
int play_score(void);
int save_score(char *);

#define SUCCESS		(0)
#define ERR_MEMORY	(1)
#define ERR_FAIL_SAVE	(2)


