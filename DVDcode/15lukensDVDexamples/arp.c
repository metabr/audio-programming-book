//arp.c
#include <stdio.h>
{
	Pm_Initialize();
	//check to make sure there are MIDI devices
	if(cnt)
	{
		//loop through input and output devices
		{
			
			//loop through number of devices
			{
				
				if(j==1 && info->output) printf("%d: %s \n", i, info->name);
			printf("choose device: ");
		//open input and output stream
		retval[0] = Pm_OpenInput(&mstream[0], dev[0], NULL, 512L, NULL,NULL);
		
		//check for error
		//if no error
		else
		{
			printf("Press ctrl-c to quit.\n\n");
			//loop forever
			{
				//if MIDI input is true
				{
					//read messages to buffer and loop through
					
					{
						//if note on
						if((Pm_MessageStatus(msg[i].message)) == MD_NOTEON)
							//loop to arpeggiate
							{	
								note = Pm_MessageData1(msg[i].message) + (j*4);
								
								//note on
								Pm_WriteShort(mstream[1], 0, Pm_Message(MD_NOTEON | chan, note, 120));
								
								
								//note on at velocity of zero to take advantage of running status
								Pm_WriteShort(mstream[1], 0, Pm_Message(MD_NOTEON | chan, note, 0));
							}
						}
			
	}
	
	Pm_Terminate();