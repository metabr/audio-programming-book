/*
 *  FFTBufferMatrix.cpp
 *  Spectral Template
 *
 *  Created by Johannes Bochmann on 11.06.09.
 *  Copyright 2009 JoboMusic GmbH. All rights reserved.
 *
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>



#include "FFTBufferMatrix.h"


FFTBufferMatrix::FFTBufferMatrix(int BufferLength, int hopframes)
{
	columnsPerFrame = hopframes;
	
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
	//append filename to documents directory
	NSString *fileSuffix = [[NSString alloc] initWithString:@"/phasesData.dtf"];
	NSString *finalPath = [documentsDirectory stringByAppendingString:fileSuffix];
	
	//our phasesFile
	phasesFile = fopen([finalPath cStringUsingEncoding:NSASCIIStringEncoding], "w+b");
	fileSuffix = @"/magData.dtf";
	finalPath = [documentsDirectory stringByAppendingString:fileSuffix];
	//our Magfile
	magFile = fopen([finalPath cStringUsingEncoding:NSASCIIStringEncoding], "w+b");
	
	//for each analysis frame we need one column of the matrix add 1 on the end to compensate for not completely filled frames
	matrixColumns = ((BufferLength/FFT_FRAMESIZE)*columnsPerFrame)+1;
	printf("matrixColumns %d\n", matrixColumns);
	
	//initialize colorMatrix and magTexture and attenuationMask
	for(int i=0; i< MATRIX_COLUMNS; i++)
	{
		for(int j=0; j< FFT_FRAMESIZE/2; j++)
		{
			//just for fun
			float scale= (float)j/(float)((float)FFT_FRAMESIZE/2.f);
			
			colorMatrix[i][j][0] = scale*6.375; //r
			colorMatrix[i][j][1] = scale*6.375; //g
			colorMatrix[i][j][2] = (1.f-scale)*6.375; //b
			colorMatrix[i][j][3] = 6.375; //a
			
			magTexture[i][j][0] = 0;
			magTexture[i][j][1] = 0;
			magTexture[i][j][2] = 0;
			magTexture[i][j][3] = 0;
			
			attenuationMask[i][j] = 1.0f;
			texAttenuationMask[i][j] = 1.0f;
			texDisplayMatrix[i][j] = 0.0f;
		}
	}
	
	//setup our bidirectional lookup-tables for the logarithmic for the logarithmic scaling
	for(int i=0; i< FFT_FRAMESIZE/2; i++) logScaleLookupArrayRevLenght[i] = 1;
	//the Rev array of logScaleLookup has n number of elements for each of FFT_FRAMESIZE/2 indeces
	logScaleLookupArrayRev = (int **) malloc(sizeof(int)*FFT_FRAMESIZE/2);
	//initialize logScale array
	int sameCounter = 1;
	int prevNumber = -1;
	int revArrayIndex = 0;
	for(int i=0; i< FFT_FRAMESIZE/2; i++)
	{
		float x= (float)i;
		
		logScaleLookupArray[i] = (int) 106.f*log10(x+1.f);
		
		if(logScaleLookupArray[i] == prevNumber)
			sameCounter++;
		else
		{
			//now we know how many elements we need 
			logScaleLookupArrayRevLenght[revArrayIndex] = sameCounter;
			logScaleLookupArrayRev[revArrayIndex] = (int*) malloc(sizeof(int)*logScaleLookupArrayRevLenght[revArrayIndex]);
			int offset = i - logScaleLookupArrayRevLenght[revArrayIndex];
			
			for(int j=0; j<logScaleLookupArrayRevLenght[revArrayIndex]; j++)
			{
				offset++;
				logScaleLookupArrayRev[revArrayIndex][j] = offset;
			}
			
			int numSkipped = 0;
			//we also have to check how many numbers we skipped
			if(prevNumber>=0)
				numSkipped = logScaleLookupArray[i] - prevNumber;
			if(numSkipped>1)
			{
				for(int j= 1; j<numSkipped; j++)
				{
					revArrayIndex++;
					logScaleLookupArrayRev[revArrayIndex] = (int*) malloc(sizeof(int));
					logScaleLookupArrayRev[revArrayIndex][0] = offset;
				}
				revArrayIndex++;
			}
			else
				revArrayIndex++;
			
			sameCounter = 1;
			prevNumber = logScaleLookupArray[i];
		}
		
	}
	
}



void FFTBufferMatrix::resetStreams()
{
	fseek(phasesFile, 0, SEEK_SET );
	fseek(magFile, 0, SEEK_SET );

}


FFTBufferMatrix::~FFTBufferMatrix()
{
	delete[] logScaleLookupArrayRev;
	delete[] magFile;
	delete[] phasesFile;
}

int FFTBufferMatrix::getMatrixColumns()
{
	return matrixColumns;
}




void FFTBufferMatrix::setMagPhasesMatrixElement(int column, int index, float real, float imaginary)
{
	float mag, phase;
	
	phase = atan2f(imaginary,real);
	mag = sqrt((real*real)+(imaginary*imaginary));
	
	//the highest range for the mag value is roughly 0.15. 
	//We scale it to be between 0 and 40 so it results in a usable value for the log
	mag = 20.0f * log10((mag*266.666f)+1.0f);
	
	fwrite(&phase, sizeof(float), 1, phasesFile);
	fwrite(&mag, sizeof(float), 1, magFile);
	fflush(phasesFile);
	fflush(magFile);
	
}



void FFTBufferMatrix::buildScaledTexture(int index, int frame, float currentMagnitude)
{
	if(index)
	{
		prevMag=displayMatrix[frame][index-1];
		//we need to interpolate over numSteps indeces
		//we simply do linear interpolation
		int numSteps = logScaleLookupArray[index]-logScaleLookupArray[index-1];
		int currentIndex = logScaleLookupArray[index-1]+1;
		float difference = currentMagnitude - prevMag;
		float step = difference/(float)numSteps;
		//linear interpolation
		float currIntpVal = prevMag+step;
		
		for(int i=0; i<numSteps;i++)
		{
			texDisplayMatrix[frame][currentIndex] = currIntpVal;
			magTexture[frame][currentIndex][0] = (GLubyte) (currIntpVal * texAttenuationMask[frame][currentIndex] * colorMatrix[frame][currentIndex][0]);
			magTexture[frame][currentIndex][1] = (GLubyte) (currIntpVal * texAttenuationMask[frame][currentIndex] * colorMatrix[frame][currentIndex][1]);
			magTexture[frame][currentIndex][2] = (GLubyte) (currIntpVal * texAttenuationMask[frame][currentIndex] * colorMatrix[frame][currentIndex][2]);
			magTexture[frame][currentIndex][3] = 1.0f; //(GLubyte) (currIntpVal*colorMatrix[frame][currentIndex][3]);
			currIntpVal+=step;
			currentIndex++;
		}
	}
	else
	{
		texDisplayMatrix[frame][logScaleLookupArray[index]] = currentMagnitude;
		magTexture[frame][logScaleLookupArray[index]][0] = (GLubyte) (currentMagnitude * texAttenuationMask[frame][logScaleLookupArray[index]] * colorMatrix[frame][index][0]);
		magTexture[frame][logScaleLookupArray[index]][1] = (GLubyte) (currentMagnitude * texAttenuationMask[frame][logScaleLookupArray[index]] * colorMatrix[frame][index][1]);
		magTexture[frame][logScaleLookupArray[index]][2] = (GLubyte) (currentMagnitude * texAttenuationMask[frame][logScaleLookupArray[index]] * colorMatrix[frame][index][2]);
		magTexture[frame][logScaleLookupArray[index]][3] = 1.0f; //(GLubyte) currentMagnitude*colorMatrix[frame][index][3];
	}
}

//in this function we fill our Matrix Frame Matrix (which is used for display) and we give back an fft_frame for resynthesis
void FFTBufferMatrix::getMagPhasesFrameFromFile(int frameNumber, float *output)
{
	int i,j,k,l, displayMatrixFrameNumber;
	float currMag, currPh;
	displayMatrixFrameNumber = frameNumber%MATRIX_COLUMNS;
	frameNumber *= columnsPerFrame;
	
	if(frameNumber%(matrixColumns-1) == 0)
		resetStreams();	//loop
	
	fread(magColumn, sizeof(float), sizeof(magColumn)/sizeof(float), magFile);
	fread(phasesColumn, sizeof(float), sizeof(phasesColumn)/sizeof(float), phasesFile);
	
	j=0;
	l=0;
	//output it
	for(i=0; i<columnsPerFrame; i++)
	{
		for( k=0; k<FFT_FRAMESIZE/2; k++, j+=2 )
		{
			currMag = magColumn[l];
			currPh = phasesColumn[l];
		
			if(!i)
			{
				//save unedited state we need when bypassing modifications
				displayMatrix[displayMatrixFrameNumber][k] = currMag;
				//build column of texture
				buildScaledTexture(k, displayMatrixFrameNumber, currMag);
			}
			
			//do the processing here
			//scale the magnitude
			currMag *= attenuationMask[displayMatrixFrameNumber][k];
			
			//convert from log scale back to linear
			currMag = pow(10.0f, currMag/20.0f) - 1.0f;
			currMag *= 0.00375f;
			//convert from polar to rectangular
			output[j] = currMag * cos(currPh);
			output[j+1] = currMag * sin(currPh);
			l++;
		}
	}
	
}


void FFTBufferMatrix::setScaledAttenuation(int column, int index, float value)
{
	for(int i=0; i<10; i++)
	{
		if( (column+i) >=MATRIX_COLUMNS) {printf("out of bound\n"); return;}
		for(int j=0; j<10; j++)
		{
			if( (index+j) < FFT_FRAMESIZE/2)
			{
				colorMatrix[column+i][index+j][0] = 6.375f;
				colorMatrix[column+i][index+j][1] = 0.f;
				colorMatrix[column+i][index+j][2] = 0.f;
				
				texAttenuationMask[column+i][index+j] = value;
				
				magTexture[column+i][index+j][0] = (GLubyte) (texDisplayMatrix[column+i][index+j] * texAttenuationMask[column+i][index+j] * colorMatrix[column+i][index+j][0]);
				magTexture[column+i][index+j][1] = (GLubyte) (texDisplayMatrix[column+i][index+j] * texAttenuationMask[column+i][index+j] * colorMatrix[column+i][index+j][1]);
				magTexture[column+i][index+j][2] = (GLubyte) (texDisplayMatrix[column+i][index+j] * texAttenuationMask[column+i][index+j] * colorMatrix[column+i][index+j][2]);
				
				for(int k=0; k<logScaleLookupArrayRevLenght[index+j]; k++)
					attenuationMask[column+i][logScaleLookupArrayRev[index+j][k]] = value;
			}
		}
	}
}


void FFTBufferMatrix::resetAttenuation()
{
	//clear all the modifications
	for(int i=0; i < MATRIX_COLUMNS; i++)
	{
		for(int j=0; j < FFT_FRAMESIZE/2; j++)
		{
			//reset attenuation masks
			attenuationMask[i][j] = 1.f;
			texAttenuationMask[i][j] = 1.f;
			
			//reset color matrix
			float scale= (float)j/(float)((float)FFT_FRAMESIZE/2.f);
			colorMatrix[i][j][0] = scale*6.375; //r
			colorMatrix[i][j][1] = scale*6.375; //g
			colorMatrix[i][j][2] = (1.f-scale)*6.375; //b
			colorMatrix[i][j][3] = 6.375; //a
			
			//redraw texture
			magTexture[i][i][0] = (GLubyte) (texDisplayMatrix[i][j] * texAttenuationMask[i][j] * colorMatrix[i][j][0]);
			magTexture[i][j][1] = (GLubyte) (texDisplayMatrix[i][j] * texAttenuationMask[i][j] * colorMatrix[i][j][1]);
			magTexture[i][j][2] = (GLubyte) (texDisplayMatrix[i][j] * texAttenuationMask[i][j] * colorMatrix[i][j][2]);
		}
	}
}














