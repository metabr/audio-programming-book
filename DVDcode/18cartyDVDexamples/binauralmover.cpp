/* 
binauralmover.cpp:
Copyright (C) Brian Carty 2009
Binaural sound source movement using magnitude interpolation and phase truncation: main program 
See license.txt for a disclaimer of all warranties and licensing information
*/

#include "defs.h"

int main()
{
	/* MIT Kemar info. */
	int elevationarray[14] = {56,60,72,72,72,72,72,60,56,45,36,24,12,1};	
	int minelev = -40,elevincrement = 10;
	/* iterators */
	int i,j;

	/* data file pointers */
	FILE *hrtfleft,*hrtfright;

	/* arrays to store addresses of where all left and right hrtfs are stored: arrays of pointers to double. */
	double *hrtfarrayl[14][37],*hrtfarrayr[14][37];
	/* pointers to read arrays */
	double *hrtfpl, *hrtfpr;	

	/* declarations for movement */
	int countbkp = 0;
	int percentages[maxbrkpts];
	double elevs[maxbrkpts],angles[maxbrkpts],elev,angle;
	int k = 0;
	int x,start = 0,sum = 0;
	
	/* crossfade preparation and checks */
	double elevindexstore,angleindexlowstore,angleindexhighstore;
	int elevindex,angleindex,oldelevindex = -1,oldangleindex = -1;		
	int fade,fadebuffer;
	int crossfade,crossout = 0,cross = 0,l = 0;

	/* interpolation variable declaration */
	int elevindexlow,elevindexhigh,angleindex1,angleindex2,angleindex3,angleindex4;
	double elevindexhighper,angleindex2per,angleindex4per;
	double magllow,magrlow,maglhigh,magrhigh,magl,magr,phasel,phaser;
	double lowl1[irlength],lowr1[irlength],lowl2[irlength],lowr2[irlength];
	double highl1[irlength],highr1[irlength],highl2[irlength],highr2[irlength];
	double hrtflinterp[irlength],hrtfrinterp[irlength],hrtfltd[irlength],hrtfrtd[irlength];
	double hrtflpadtd[irpadlength],hrtfrpadtd[irpadlength],hrtflpadspec[irpadlength],hrtfrpadspec[irpadlength];

	/* convolution/in/output buffers */
	double inbuf[irpadlength],inspec[irpadlength];
	double outlspec[irpadlength],outrspec[irpadlength],outl[irpadlength] = {0.0},outr[irpadlength] = {0.0};  
	double overlapl[overlapsize],overlapr[overlapsize];
	sf_count_t count=0;
	double lrout[2*irlength];

	/* various buffers for fades */
	double currentphasel[irlength],currentphaser[irlength];
	double hrtflpadspecold[irpadlength],hrtfrpadspecold[irpadlength];			
	double outlspecold[irpadlength],outrspecold[irpadlength];						
	double overlaplold[overlapsize],overlaprold[overlapsize];	
	double outlold[irpadlength] = {0.0},outrold[irpadlength] = {0.0};			

	/* file pointers, file info */
	char filename[100];
	SNDFILE *fin, *fout;					
	SF_INFO *psfinfoout, *psfinfoin;

	/* fftw plans */
	fftw_plan invhrtfl,invhrtfr,forhrtflpad,forhrtfrpad,forin;
	fftw_plan invoutl,invoutr,invoutlold,invoutrold;

	invhrtfl = fftw_plan_r2r_1d(irlength, hrtflinterp, hrtfltd, FFTW_HC2R, FFTW_ESTIMATE); 
	invhrtfr = fftw_plan_r2r_1d(irlength, hrtfrinterp, hrtfrtd, FFTW_HC2R, FFTW_ESTIMATE); 
	forhrtflpad = fftw_plan_r2r_1d(irpadlength, hrtflpadtd, hrtflpadspec, FFTW_R2HC, FFTW_ESTIMATE); 
	forhrtfrpad = fftw_plan_r2r_1d(irpadlength, hrtfrpadtd, hrtfrpadspec, FFTW_R2HC, FFTW_ESTIMATE);
	forin = fftw_plan_r2r_1d(irpadlength, inbuf, inspec, FFTW_R2HC, FFTW_ESTIMATE);
	invoutl = fftw_plan_r2r_1d(irpadlength, outlspec, outl, FFTW_HC2R, FFTW_ESTIMATE); 
	invoutr = fftw_plan_r2r_1d(irpadlength, outrspec, outr, FFTW_HC2R, FFTW_ESTIMATE); 
	invoutlold = fftw_plan_r2r_1d(irpadlength, outlspecold, outlold, FFTW_HC2R, FFTW_ESTIMATE); 
	invoutrold = fftw_plan_r2r_1d(irpadlength, outrspecold, outrold, FFTW_HC2R, FFTW_ESTIMATE); 

	/* memory for SF_INFO structures */
	psfinfoin = new SF_INFO;	
	psfinfoout = new SF_INFO; 

	printf("\nBinaural Processing Application\n\n");

	/* setup crossfades: over user defined number of convolution cycles */
	printf("enter number of processing buffers for fades (>1),8 is good for musical source,less for noisy sources:\n");
	scanf("%d",&fade);
	if(fade <= 0)
	{ 
		printf("fade number must be positive, exiting\n"); 
		exit(1);
	}
	if(fade > 24)
		fade = 24;
	fadebuffer = fade * irlength;

	printf("enter mono (wav) sound file,include.wav extension(<100 characters):\n");
	scanf("%s",filename);

	/* open files */
	if(!(fin = sf_open(filename, SFM_READ, psfinfoin)))
	{ 
		printf("error opening in file, exiting\n"); 
		exit(1);
	}

	if(psfinfoin->channels != 1)
	{ 
		printf("input should be mono, exiting\n"); 
		exit(1);
	}

	if(!(hrtfleft = fopen("datal.raw","rb")))	
	{
		printf("error opening hrtf file,exiting\n");
		exit(1);
	}

	if(!(hrtfright = fopen("datar.raw","rb")))
	{
		printf("error opening hrtf file,exiting\n");
		exit(1);
	}

	/* store files */ 
	for(i = 0; i < 14; i++)
		for(j = 0; j < elevationarray[i] / 2 + 1; j++)
		{
			/* hrtfarray[i][j] = &hrtfarray[i][j][0] */
			hrtfarrayl[i][j] = new double [irlength];			
			hrtfarrayr[i][j] = new double [irlength];
			fread(hrtfarrayl[i][j],sizeof(double),irlength,hrtfleft);		
			fread(hrtfarrayr[i][j],sizeof(double),irlength,hrtfright);
		}

	/* initialise the SF_INFO structure (need to do this before opening file!), same as input but stereo */ 
	psfinfoout->samplerate = psfinfoin->samplerate; 
	psfinfoout->channels = 2; 
	psfinfoout->format = psfinfoin->format; 
		
	if(!(fout = sf_open("mover.wav", SFM_WRITE, psfinfoout)))
	{ 
		printf("error opening out file\n"); 
		exit(1);
	}
	
	/* function to read, check and store trajectory */
	bkpt(percentages, elevs, angles, &countbkp, maxbrkpts);

	printf("...\nprocessing\n...\n");

	/* main loop */
	for(x = 0; x < countbkp; x++)		
	{
		start = sum;
		/* run to full length of convolved output */
		sum = (int)((psfinfoin->frames + irlength - 1) * percentages[x + 1] / 100.0);

		do		
		{
			crossout = 0;
			crossfade = 0;

			/* change elev and angle according to bkpt file */
			elev = elevs[x] + (elevs[x + 1] - elevs[x]) * (double)(k - start) / (sum - start);
			angle = angles[x] + (angles[x + 1] - angles[x]) * (double)(k - start) / (sum - start);
	
			/* two nearest elev indices */
			/* to avoid recalculating  */
			elevindexstore = (elev - minelev) / elevincrement;
			elevindexlow = (int)elevindexstore;

			if(elevindexlow < 13) 
				elevindexhigh = elevindexlow + 1;
			else 
				elevindexhigh = elevindexlow;		/* highest index reached */

			/* get percentage value for interpolation */
			elevindexhighper = elevindexstore - elevindexlow;

			while(angle < 0.0)
				angle += 360.0;
			while(angle >= 360.0)
				angle -= 360.0;
	
			/* as above,lookup index, used to check for crossfade */
			elevindex = (int)(elevindexstore + 0.5);

			angleindex = (int)(angle / (360.0 / elevationarray[elevindex]) + 0.5);
			angleindex = angleindex % elevationarray[elevindex];

			/* crossfade happens if index changes:nearest measurement changes */
			if (oldelevindex != elevindex || oldangleindex != angleindex)	
			{ 
				if(k > 0)
				{
					/* warning on overlapping fades */
					if(cross)
					{
						printf("\nwarning: fades are overlapping: this could lead to noise: reduce fade size or change trajectory");
						cross = 0;
					}
					/* reset l */
					l = 0;
					crossfade = 1;
					for(i = 0; i < irpadlength; i++)
					{
						hrtflpadspecold[i] = hrtflpadspec[i];				
						hrtfrpadspecold[i] = hrtfrpadspec[i];
					}
				}

				if(angleindex > elevationarray[elevindex] / 2)
				{
					hrtfpl = hrtfarrayl[elevindex][elevationarray[elevindex] - angleindex];
					hrtfpr = hrtfarrayr[elevindex][elevationarray[elevindex] - angleindex];
					for(i = 0; i < irlength; i++)	
					{
						currentphasel[i]=hrtfpr[i];
						currentphaser[i]=hrtfpl[i];
					}
				}
				else
				{
					hrtfpl = hrtfarrayl[elevindex][angleindex];
					hrtfpr = hrtfarrayr[elevindex][angleindex];
					for(i = 0; i < irlength; i++)
					{
						currentphasel[i]=hrtfpl[i];
						currentphaser[i]=hrtfpr[i];
					}
				}
			}

			/* avoid recalculation */
			angleindexlowstore = angle / (360.0 / elevationarray[elevindexlow]);
			angleindexhighstore = angle / (360.0 / elevationarray[elevindexhigh]);
	
			/* 4 closest indices, 2 low and 2 high */
			angleindex1 = (int)angleindexlowstore;

			angleindex2 = angleindex1 + 1;
			angleindex2 = angleindex2 % elevationarray[elevindexlow];

			angleindex3 = (int)angleindexhighstore;

			angleindex4 = angleindex3 + 1;
			angleindex4 = angleindex4 % elevationarray[elevindexhigh];

			/* angle percentages for interp */
			angleindex2per = angleindexlowstore - angleindex1;
			angleindex4per = angleindexhighstore - angleindex3;

			/* read 4 nearest HRTFs  */
			/* switch l and r */
			if(angleindex1 > elevationarray[elevindexlow] / 2)
			{
				hrtfpl = hrtfarrayl[elevindexlow][elevationarray[elevindexlow] - angleindex1];
				hrtfpr = hrtfarrayr[elevindexlow][elevationarray[elevindexlow] - angleindex1];
				for(i = 0; i < irlength; i++)
				{
					lowl1[i] = hrtfpr[i];
					lowr1[i] = hrtfpl[i];
				}
			}
			else
			{
				hrtfpl = hrtfarrayl[elevindexlow][angleindex1];
				hrtfpr = hrtfarrayr[elevindexlow][angleindex1];
				for(i = 0; i < irlength; i++)
				{
					lowl1[i] = hrtfpl[i];
					lowr1[i] = hrtfpr[i];
				}
			}

			if(angleindex2 > elevationarray[elevindexlow] / 2)
			{
				hrtfpl = hrtfarrayl[elevindexlow][elevationarray[elevindexlow] - angleindex2];
				hrtfpr = hrtfarrayr[elevindexlow][elevationarray[elevindexlow] - angleindex2];
				for(i = 0; i < irlength; i++)
				{
					lowl2[i] = hrtfpr[i];
					lowr2[i] = hrtfpl[i];
				}
			}
			else
			{
			hrtfpl = hrtfarrayl[elevindexlow][angleindex2];
			hrtfpr = hrtfarrayr[elevindexlow][angleindex2];
				for(i=0; i<irlength; i++)
				{
					lowl2[i] = hrtfpl[i];
					lowr2[i] = hrtfpr[i];
				}
			}

			if(angleindex3 > elevationarray[elevindexhigh] / 2)
			{
				hrtfpl = hrtfarrayl[elevindexhigh][elevationarray[elevindexhigh] - angleindex3];
				hrtfpr = hrtfarrayr[elevindexhigh][elevationarray[elevindexhigh] - angleindex3];
				for(i = 0; i < irlength; i++)
				{
					highl1[i] = hrtfpr[i];
					highr1[i] = hrtfpl[i];
				}
			}
			else
			{
				hrtfpl = hrtfarrayl[elevindexhigh][angleindex3];
				hrtfpr = hrtfarrayr[elevindexhigh][angleindex3];
				for(i = 0; i < irlength; i++)
				{
					highl1[i] = hrtfpl[i];
					highr1[i] = hrtfpr[i];
				}
			}

			if(angleindex4 > elevationarray[elevindexhigh] / 2)
			{
				hrtfpl = hrtfarrayl[elevindexhigh][elevationarray[elevindexhigh] - angleindex4];
				hrtfpr = hrtfarrayr[elevindexhigh][elevationarray[elevindexhigh] - angleindex4];
				for(i = 0; i < irlength; i++)
				{
					highl2[i] = hrtfpr[i];
					highr2[i] = hrtfpl[i];
				}
			}
			else
			{
				hrtfpl = hrtfarrayl[elevindexhigh][angleindex4];
				hrtfpr = hrtfarrayr[elevindexhigh][angleindex4];
				for(i = 0; i < irlength; i++)
				{
					highl2[i] = hrtfpl[i];
					highr2[i] = hrtfpr[i];
				}
			}

			/* magnitude interpolation */
			/* 0hz and Nyq real values */
			/* organised in format of fftw */
			magllow = fabs(lowl1[0]) + (fabs(lowl2[0]) - fabs(lowl1[0])) * angleindex2per;
			maglhigh = fabs(highl1[0]) + (fabs(highl2[0]) - fabs(highl1[0])) * angleindex4per;
			magrlow = fabs(lowr1[0]) + (fabs(lowr2[0]) - fabs(lowr1[0])) * angleindex2per;
			magrhigh = fabs(highr1[0]) + (fabs(highr2[0]) - fabs(highr1[0])) * angleindex4per;
			magl = magllow + (maglhigh - magllow) * elevindexhighper;
			magr = magrlow + (magrhigh - magrlow) * elevindexhighper;
			if(currentphasel[0] < 0.0)
				hrtflinterp[0] = -magl;
			else
				hrtflinterp[0] = magl;
			if(currentphaser[0] < 0.0)
				hrtfrinterp[0] = -magr;
			else
				hrtfrinterp[0] = magr;

			magllow = fabs(lowl1[1]) + (fabs(lowl2[1]) - fabs(lowl1[1])) * angleindex2per;
			maglhigh = fabs(highl1[1]) + (fabs(highl2[1]) - fabs(highl1[1])) * angleindex4per;
			magrlow = fabs(lowr1[1]) + (fabs(lowr2[1]) - fabs(lowr1[1])) * angleindex2per;
			magrhigh = fabs(highr1[1]) + (fabs(highr2[1]) - fabs(highr1[1])) * angleindex4per;
			magl = magllow + (maglhigh - magllow) * elevindexhighper;
			magr = magrlow + (magrhigh - magrlow) * elevindexhighper;
			if(currentphasel[1] < 0.0)
				hrtflinterp[irlength/2] = -magl;
			else
				hrtflinterp[irlength/2] = magl;
			if(currentphaser[1] < 0.0)
				hrtfrinterp[irlength/2] = -magr;
			else
				hrtfrinterp[irlength/2] = magr;

			/* other values are complex, in fftw format */
			for(i = 2, j=1; i < irlength; j++, i+=2)
			{	
				/* interpolate high and low magnitudes */
				magllow = lowl1[i] + (lowl2[i] - lowl1[i]) * angleindex2per;
				maglhigh = highl1[i] + (highl2[i] - highl1[i]) * angleindex4per;

				magrlow = lowr1[i] + (lowr2[i] - lowr1[i]) * angleindex2per;
				magrhigh = highr1[i] + (highr2[i] - highr1[i]) * angleindex4per;
	
				/* interpolate high and low results,use current phase */
				magl = magllow +  (maglhigh - magllow) * elevindexhighper;
				phasel = currentphasel[i + 1];
			
				/* polar to rectangular, organised in fftw order */
				hrtflinterp[j] = magl * cos(phasel);
				hrtflinterp[irlength - j] = magl * sin(phasel);

				magr = magrlow + (magrhigh - magrlow) * elevindexhighper;
				phaser = currentphaser[i + 1];
 
				hrtfrinterp[j] = magr * cos(phaser);
				hrtfrinterp[irlength - j] = magr * sin(phaser);
			} 

			fftw_execute(invhrtfl);
			fftw_execute(invhrtfr);

			/* scale and pad */
			for(i = 0; i < irlength; i++)
			{
				hrtflpadtd[i] = (hrtfltd[i] / irlength) * .65;	
				hrtfrpadtd[i] = (hrtfrtd[i] / irlength) * .65;
			}	

			for(i = irlength; i < irpadlength; i++)
			{
				hrtflpadtd[i] = 0.0;
				hrtfrpadtd[i] = 0.0;
			}

			/* execute fft on padded hrtfs */
			fftw_execute(forhrtflpad);
			fftw_execute(forhrtfrpad);

			/* look after overlap add */
			for(i = 0; i < overlapsize ; i++)
			{
				overlapl[i] = outl[i+irlength];
				overlapr[i] = outr[i+irlength];
				if(crossfade)
				{
					overlaplold[i] = outl[i+irlength];
					overlaprold[i] = outr[i+irlength];
				}
				/* overlap will be previous fading out signal */
				if(cross)
				{
					overlaplold[i] = outlold[i+irlength];
					overlaprold[i] = outrold[i+irlength];
				}
			}
		
			/* read input */
			count = sf_readf_double(fin, inbuf, irlength);
		
			/* zero pad */
			/* fills last one with zeros from count */
			for(i = (int)count; i < irpadlength; i++)		
				inbuf[i] = 0.0;	

			/* fft input */
			fftw_execute(forin);
		
			/* convolution: spectral multiplication */
			/* 0hz and Nyq */
			outlspec[0] = inspec[0] * hrtflpadspec[0];
			outrspec[0] = inspec[0] * hrtfrpadspec[0];
			outlspec[irpadlength/2] = inspec[irpadlength/2] * hrtflpadspec[irpadlength/2];
			outrspec[irpadlength/2] = inspec[irpadlength/2] * hrtfrpadspec[irpadlength/2];

			/* complex multiplication according to fftw layout */
			/* (a + i b)(c + i d) */
			/* = (a c - b d) + i(a d + b c) */
			for(i = 2, j = 1; i < irpadlength; j++, i+=2) 
			{
				/* real */
				outlspec[j] = inspec[j] * hrtflpadspec[j] - inspec[irpadlength - j] * hrtflpadspec[irpadlength - j];
				outrspec[j] = inspec[j] * hrtfrpadspec[j] - inspec[irpadlength - j] * hrtfrpadspec[irpadlength - j];
				/* imaginary */
				outlspec[irpadlength - j] = inspec[j] * hrtflpadspec[irpadlength - j] + inspec[irpadlength - j] * hrtflpadspec[j];
				outrspec[irpadlength - j] = inspec[j] * hrtfrpadspec[irpadlength - j] + inspec[irpadlength - j] * hrtfrpadspec[j];
			}

			fftw_execute(invoutl);
			fftw_execute(invoutr);

			/* scaled, as fftw is a sum */
			for(i = 0; i < irpadlength; i++)
			{
				outl[i] = outl[i] / irpadlength;		
				outr[i] = outr[i] / irpadlength;	
			}

			/* setup for fades */
			if(crossfade || cross)	
			{		
				crossout = 1;

				/* convolution */
				/* 0hz and Nyq */
				outlspecold[0] = inspec[0] * hrtflpadspecold[0];
				outrspecold[0] = inspec[0] * hrtfrpadspecold[0];
				outlspecold[irpadlength/2] = inspec[irpadlength/2] * hrtflpadspecold[irpadlength/2];
				outrspecold[irpadlength/2] = inspec[irpadlength/2] * hrtfrpadspecold[irpadlength/2];

				/* complex multiplication */
				for(i = 2, j = 1; i < irpadlength; j++, i+=2) 
				{
					/* real */
					outlspecold[j] = inspec[j] * hrtflpadspecold[j] - inspec[irpadlength - j] * hrtflpadspecold[irpadlength - j];
					outrspecold[j] = inspec[j] * hrtfrpadspecold[j] - inspec[irpadlength - j] * hrtfrpadspecold[irpadlength - j];
					/* imaginary */
					outlspecold[irpadlength - j] = inspec[j] * hrtflpadspecold[irpadlength - j] + inspec[irpadlength - j] * hrtflpadspecold[j];
					outrspecold[irpadlength - j] = inspec[j] * hrtfrpadspecold[irpadlength - j] + inspec[irpadlength - j] * hrtfrpadspecold[j];
				}

				/* ifft, back to time domain */
				fftw_execute(invoutlold);
				fftw_execute(invoutrold);

				/* scaling */
				for(i = 0; i < irpadlength; i++)
				{
					outlold[i] = outlold[i] / irpadlength;		
					outrold[i] = outrold[i] / irpadlength;	
				}		
			
				cross++;				
				cross = cross % fade;
			}
		
			/* for next check */
			oldelevindex = elevindex; 
			oldangleindex = angleindex;
		
			if(crossout)						
				for(i = 0; i < irlength; i++)
				{
					lrout[2 * i] = (outlold[i] + (i < overlapsize ? overlaplold[i] : 0.0)) * (1.0 - (double)l / fadebuffer) +
					(outl[i] + (i < overlapsize ? overlapl[i] : 0.0)) * (double)l / fadebuffer;
					lrout[(2 * i) + 1] = (outrold[i] + (i < overlapsize ? overlaprold[i] : 0.0)) * (1.0 - (double)l / fadebuffer) +
					(outr[i] + (i < overlapsize ? overlapr[i] : 0.0)) * (double)l / fadebuffer;
					l++;
				}
			else
				for(i = 0; i < irlength; i++)
				{
					lrout[2 * i] = outl[i] + (i < overlapsize ? overlapl[i] : 0.0);
					lrout[(2 * i) + 1] = outr[i] + (i < overlapsize ? overlapr[i] : 0.0);
				}

			/* do every irlength samples! */
			k+=irlength;		
	
			/* if on last run, only write output length mod irlength frames */
			if(k > psfinfoin->frames + irlength - 1) 
				sf_writef_double(fout, lrout, (psfinfoin->frames + irlength - 1) % irlength);
			else 
				sf_writef_double(fout, lrout, irlength);
		}
		while (k<sum);
	}

	/* clear dynamic memory,close files */
	delete psfinfoin;
	delete psfinfoout; 
	sf_close(fin);
	sf_close(fout);
	fclose(hrtfleft);
	fclose(hrtfright);

	for(i = 0; i < 14; i++)
		for(j = 0; j < elevationarray[i] / 2 + 1; j++)
		{
			delete[] hrtfarrayl[i][j];
			delete[] hrtfarrayr[i][j];
		}

	fftw_destroy_plan(invhrtfl);
	fftw_destroy_plan(invhrtfr);
	fftw_destroy_plan(forhrtflpad);
	fftw_destroy_plan(forhrtfrpad);
	fftw_destroy_plan(forin);
	fftw_destroy_plan(invoutl);
	fftw_destroy_plan(invoutr);
	fftw_destroy_plan(invoutlold);
	fftw_destroy_plan(invoutrold);

	return 0;
}
















