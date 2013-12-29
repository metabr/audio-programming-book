/*
 *  main.h
 *  Filter program
 *
 * 
 *  
 *
 */

#include <Carbon/Carbon.h>


class filter
/*
 *                                                
 *
 *
 */
{

private: 	double_t h[32];    //  impulse response
			double_t H[32];    //  frequency response
			double_t delay_line[32];

public:		short read_multiply_shift(short sample);
			void perform_IDFT(int FilterType, int centre_or_cutoff_freq);

};



