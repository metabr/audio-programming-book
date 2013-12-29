#include <avr/io.h>     
#include <avr/interrupt.h>

#define PB1 1 // just in case you are using a particular version of avr-libc

void setPORTBvalue(char bitNum, char bitVal)
{
	if (bitVal)
		PORTB |= _BV(bitNum);
	else
		PORTB &= ~(_BV(bitNum));
}

void setupOutputs()
{
	DDRB = _BV(PB1);
}

typedef struct audioOutput {
	char isPlaying;
	unsigned int baseFullTime;
	unsigned int fullTime;
	unsigned int upTime;
	unsigned int count;
	char jitterAmount;
	char halfJitterAmount;
	unsigned int jitterSpeed;
	unsigned int fullJitterSpeed;
	unsigned int jitterCount;
	unsigned int lifeTime;
} audioOutput;

audioOutput out;

void triggerSound(audioOutput* output, char keepPhase,
				  unsigned int time, int pulseWidth, char jitterAmount, unsigned int jitterSpeed, unsigned int lifeTime)
{
	output->isPlaying = 0;
	if (time >= 2)
	{
		output->baseFullTime = time;
		output->fullTime = time;
		output->upTime = (pulseWidth * time) / 100;
		if (output->upTime < 1)
			output->upTime = 1;
		if (!keepPhase)
			output->count = 0;
		output->jitterAmount = jitterAmount;
		output->halfJitterAmount = jitterAmount / 2;
		if (!jitterSpeed)
			jitterSpeed = 1;
		output->jitterSpeed = jitterSpeed;
		output->fullJitterSpeed = jitterSpeed;
		output->lifeTime = lifeTime;
		output->isPlaying = 1;
	}
}

SIGNAL (TIMER0_COMPA_vect)
{
	if (out.isPlaying)
	{
		out.lifeTime--;
		if (out.lifeTime)
		{
			out.count = (out.count + 1) % out.fullTime;
			if (out.count == 0 && out.jitterAmount > 0)
			{
				out.jitterSpeed--;
				if (out.jitterSpeed == 0)
				{
					out.jitterSpeed = out.fullJitterSpeed;
					out.fullTime = out.baseFullTime + (rand() % out.jitterAmount) - out.halfJitterAmount;
				}
			}
			
			if (out.count < out.upTime)
				setPORTBvalue(PB1, 1);
			else
				setPORTBvalue(PB1, 0);
		}
		else
		{
			out.isPlaying = 0;
			setPORTBvalue(PB1, 0);
		}
	}
}

#define sequenceSize 16
#define numPitches 8

unsigned long maxSteps = 10240;
unsigned long numSteps = 10240;
int seqSize = sequenceSize;
int seqIndex = 0;

int sequence[sequenceSize][4]; // 4 parameters for each sequence position
int pitches[numPitches] = {2, 4, 8, 16, 6, 12, 18, 24};


SIGNAL (TIMER1_COMPA_vect)
{
	if (numSteps >= maxSteps)
	{
		int i;
		
		for (i=0; i<seqSize; i++)
		{
			if (rand()%100 < 25) // 25% chance of silence
				sequence[i][0] = 0;
			else
			{
				sequence[i][0] = pitches[rand()%numPitches]; // random pitch
				sequence[i][1] = rand()%50 + 49;			// random pulsewidth
				sequence[i][2] = rand()%20;					// random jitterAmount
				sequence[i][3] = rand()%20;					// random jitterSpeed
			}
		}
		
		OCR1A = rand() % 500 + 11;
		numSteps = 0;
	}
	
	
	seqIndex = (seqIndex + 1) % seqSize;
	triggerSound(&out, 0, sequence[seqIndex][0], sequence[seqIndex][1], sequence[seqIndex][2], sequence[seqIndex][3], 
				 (OCR1A + 1) * 2);	// Difference between timer0 and the base value of timer1 is 2
	
	numSteps -= OCR1A;
}

int main()
{
	setupOutputs();
	
	sei();
	
	// Audio rate timer
	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01) | _BV(CS00); // (CLK / 64)
	OCR0A = 31;// CLK/64/32 = 2048
	TIMSK0 = _BV(OCIE0A);
	
	// Sequencer timer
	TCCR1A = _BV(WGM12);
	TCCR1B = (_BV(WGM12) | _BV(CS12) | _BV(CS10)); // (CLK/1024)
	OCR1A = 2047; // CLK/1024/1024 = CLK/2097152
	TIMSK1 = _BV(OCIE1A);
	
	while (1)
	{
	}
	
	return 1;
}
