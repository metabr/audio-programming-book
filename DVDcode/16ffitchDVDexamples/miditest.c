#include <stdio.h>
#include "midilib.h"

int main(void)
{
    new_score();
    add_note(60, 64, 0, 0, 1000);
    add_note(62, 64, 0, 1000, 1000);
    add_note(64, 64, 0, 2000, 1000);
    add_note(65, 64, 0, 3000, 1000);
    add_note(67, 64, 0, 4000, 1000);
    add_note(69, 64, 0, 5000, 1000);
    add_note(71, 64, 0, 6000, 1000);
    add_note(72, 64, 0, 7000, 1000);
    save_score("miditest.mid");
}
