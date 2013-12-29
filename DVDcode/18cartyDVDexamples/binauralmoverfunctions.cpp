/* 
binauralmoverfunctions.cpp:
Copyright (C) Brian Carty 2009
Binaural sound source movement using magnitude interpolation and phase truncation: function for text file read
See license.txt for a disclaimer of all warranties and licensing information
*/

#include "defs.h"

void bkpt(int *pers, double *els, double *angs, int *noofpoints, int maxpts)		
{
	/* file details */
	FILE *finbkp;
	char bkpfilename[100];
	int i;

	printf("enter breakpoint file (integer value percentages),include.txt extension (<100 characters):\n");
	scanf("%s",bkpfilename);

	if(!(finbkp = fopen(bkpfilename,"r")))
	{ 
		printf("error opening breakpoint file, exiting\n"); 
		exit(1);
	}

	for(i = 0; i < maxpts; i++)
	{
		/* read input from file */
		if(!feof(finbkp))
		{
			fscanf(finbkp,"%d",&pers[i]);
			fscanf(finbkp,"%lf",&els[i]);
			if(els[i] > 90.0)
				els[i] = 90.0;
			if(els[i] < -40.0)
				els[i] = -40.0;
			fscanf(finbkp,"%lf",&angs[i]);

			/* do checks */
			/* legal % values ? */
			if(pers[i] > 100 || pers[i] < 0)
			{
				printf("error, breakpoint file must run from 0 to 100, exiting\n");
				exit(1);
			}
			/* percentage accumulation */
			if(i > 0 && pers[i] <= pers[i - 1])
			{
				printf("error, percentage values must accumulate...%d is not > %d, exiting\n",pers[i],pers[i-1]);
				exit(1);
			}

			/* end at 100% */
			if(pers[i] == 100)
			break;

		*noofpoints = *noofpoints + 1;
		}
		else 
		break;
	}

	/* check last value is 100 */ 
	if(pers[*noofpoints] != 100)
	{
		printf("error, percentage values must conclude with 100, not %d, exiting",pers[i]);
		exit(1);
	}

	/* close file */
	fclose(finbkp);
}

