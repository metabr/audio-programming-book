/*
 *  FFTBufferMatrix.h
 *  Spectral Template
 *
 *  Created by Johannes Bochmann on 11.06.09.
 *  Copyright 2009 JoboMusic GmbH. All rights reserved.
 *
 */

#ifndef __FFTBufferMatrix_h__
#define __FFTBufferMatrix_h__


#import <OpenGLES/ES1/gl.h>
#include "globals.h"

	
class FFTBufferMatrix {
	
private:
	
	FILE *magFile;
	FILE *phasesFile;
	float magColumn[(FFT_FRAMESIZE/2)*(FFT_FRAMESIZE/HOPSIZE)];
	float phasesColumn[(FFT_FRAMESIZE/2)*(FFT_FRAMESIZE/HOPSIZE)];
	
	//raw mag values after processing
	float displayMatrix[MATRIX_COLUMNS][FFT_FRAMESIZE/2];
	float texDisplayMatrix[MATRIX_COLUMNS][FFT_FRAMESIZE/2];
	//raw color data for the texture
	float colorMatrix[MATRIX_COLUMNS][FFT_FRAMESIZE/2][4];
	//attenuation of each mag value between 0.0 and 1.0
	float attenuationMask[MATRIX_COLUMNS][FFT_FRAMESIZE/2];
	float texAttenuationMask[MATRIX_COLUMNS][FFT_FRAMESIZE/2];
	
	int logScaleLookupArray[FFT_FRAMESIZE/2];
	int **logScaleLookupArrayRev;
	int logScaleLookupArrayRevLenght[FFT_FRAMESIZE/2];
	
	float prevMag;
	int matrixColumns;
	int columnsPerFrame;
	
public:
	//resulting texture
	GLubyte magTexture[MATRIX_COLUMNS][FFT_FRAMESIZE/2][4];
	
	FFTBufferMatrix(int BufferLength, int hopframes);
	~FFTBufferMatrix();
	
	void resetStreams();

	int getMatrixColumns();
	
	void setMagPhasesMatrixElement(int column, int index, float real, float imaginary);
			
	void getMagPhasesFrameFromFile(int frameNumber, float *output);
	
	void buildScaledTexture(int index, int frame, float currentMagnitude);
	
	void setScaledAttenuation(int column, int index, float value);
	
	void resetAttenuation();
	
	
};

#endif //__FFTBufferMatrix_h__