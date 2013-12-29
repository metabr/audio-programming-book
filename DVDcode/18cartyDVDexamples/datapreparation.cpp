/* 
datapreparation.cpp:
Copyright (C) Brian Carty 2009
Binaural sound source movement using magnitude interpolation and phase truncation: MIT HRTF data preparation 
See license.txt for a disclaimer of all warranties and licensing information
*/

#include "defs.h"

int main()
{
	/* setup variables */
	/* min elev, angle, increment, iterators */
	int el=-40,az=0,inc,i,j,k;
	/* input from HRTF file */
	double input[2*irlength];
	/* separate input into left and right */
	double inl[irlength],inr[irlength],fftl[irlength],fftr[irlength];
	/* file pointers */
	FILE *foutl,*foutr;
	/* fft plans */
	fftw_plan forwardl,forwardr;		
	/* strings for filename */
	char filename[14];
	char hrtffile[22];
	/* file in pointer */
	SNDFILE *finhrtf;
	/* file info */
	SF_INFO *psfinfohrtf;				
	/* memory for file info */
	psfinfohrtf = new SF_INFO;

	/* setup fft plans (see fftw documentation) */
	forwardl = fftw_plan_r2r_1d(irlength, inl, fftl, FFTW_R2HC, FFTW_ESTIMATE); 
	forwardr = fftw_plan_r2r_1d(irlength, inr, fftr, FFTW_R2HC, FFTW_ESTIMATE); 

	/* open outfiles for writing */
	foutl=fopen("datal.raw","wb");	
	foutr=fopen("datar.raw","wb");		

	/* loop for 368 files */
	for(j = 0; j < 368; j++)
	{	
		/* prep for file open string */
		strcpy(hrtffile,"diffuse/");

		/* prep file names */
		if(az < 10)
			sprintf(filename,"H%de00%da.wav",el,az);
		else if(az >= 10 && az < 100)
			sprintf(filename,"H%de0%da.wav",el,az);
		else if(az >= 100)
			sprintf(filename,"H%de%da.wav",el,az);

		/* sort out incrementation based on elev */
		if(el == -40)
		{
			if(inc != 6 || j % 7 == 0)
				inc = 6; 
			else inc = 7;
		}			
		else if(el == -30 || el == 30)
			inc = 6;	
		else if(el == -20 || el == -10 || el == 0 || el == 10 || el == 20)
			inc = 5;
		else if(el == 40)
		{
			if(inc != 6 || (j - 276) % 7 == 0)
				inc = 6; 
			else inc = 7;
		}
		else if(el == 50)
			inc = 8;		
		else if(el == 60)
			inc = 10;		
		else if(el == 70)
			inc = 15;
		else if(el == 80)
			inc = 30;		
		else if(el == 90)
			inc = 0;

		/* put together for full name */
		strcat(hrtffile,filename);

		/* open appropriate file */
		if(!(finhrtf = sf_open(hrtffile, SFM_READ, psfinfohrtf)))
		{ 
			printf("error opening file\n"); 
			exit(1);
		}		

		/* read in file */
		sf_readf_double(finhrtf, input, irlength);
		/* close file */
		sf_close(finhrtf);

		/* put (double: -1.0 to +1.0) input into seperate left and right buffers */
		for(i = 0; i < irlength; i++)
		{
			inl[i] = input[2 * i];	
			inr[i] = input[(2 * i) + 1];
		}

		/* fft */
		fftw_execute(forwardl);
		fftw_execute(forwardr);

		/* 0Hz and nyq */
		inl[0] = fftl[0];
		inl[1] = fftl[irlength / 2];
		inr[0] = fftr[0];
		inr[1] = fftr[irlength / 2];

		/* mag/phase: polar */
		for(i = 2, k = 1; i < irlength; k++, i+=2) 
		{
			inl[i] = sqrt(pow(fftl[k],2) + pow(fftl[irlength - k],2));
			inl[i+1] = atan2(fftl[irlength-k],fftl[k]);
			inr[i] = sqrt(pow(fftr[k],2) + pow(fftr[irlength - k],2));
			inr[i+1] = atan2(fftr[irlength-k],fftr[k]);
		}

		/* write outputs, one by one, to large spectral file */
		fwrite(inl,sizeof(double),irlength,foutl);
		fwrite(inr,sizeof(double),irlength,foutr);		

		/* incrementation */
		az = az + inc;

		if(j == 28 || j == 59 || j == 96 || j == 133 || j == 170 || j == 207 || j == 244 || j == 275 || j == 304 || j == 327 || j == 346 || j == 359 || j == 366)
		{
			/* change elevation,reset variables */
			el = el + 10;			
			az = 0;
			inc = 0;	
		}				
	}
	/* clear memory, close files */
	delete psfinfohrtf;		
	fclose(foutl);
	fclose(foutr);	
	fftw_destroy_plan(forwardl);
	fftw_destroy_plan(forwardr);

	return 0;
}


