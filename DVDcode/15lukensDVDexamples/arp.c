//arp.c
#include <stdio.h>#include "include/portmidi.h"#include "include/porttime.h"#define MD_NOTEON 0x90 //hex value for note onint main()
{	int cnt, i, dev[2], j, note;	PmError retval[2];  	const PmDeviceInfo *info;	PmEvent msg[32];	long time;	PortMidiStream *mstream[2];	
	Pm_Initialize();	cnt = Pm_CountDevices();	
	//check to make sure there are MIDI devices
	if(cnt)
	{
		//loop through input and output devices		for(j=0; j<2; j++)
		{			if(j==0) printf("input devices:\n");			else printf("output devices:\n");
			
			//loop through number of devices			for(i=0; i < cnt; i++)
			{				info = Pm_GetDeviceInfo(i);
								if(j==0 && info->input) printf("%d: %s \n", i, info->name);
				if(j==1 && info->output) printf("%d: %s \n", i, info->name);			}		
			printf("choose device: ");			scanf("%d", &dev[j]); 		}		Pt_Start(1, NULL, NULL); //start porttime		
		//open input and output stream
		retval[0] = Pm_OpenInput(&mstream[0], dev[0], NULL, 512L, NULL,NULL);		retval[1] = Pm_OpenOutput(&mstream[1], dev[1], NULL, 512L, NULL,NULL,0L);
		
		//check for error		if(retval[0] != pmNoError && retval[0] != pmNoError)    			printf("error in input device (%s) or in output device (%s) \n", Pm_GetErrorText(retval[0]), Pm_GetErrorText(retval[1]) );		
		//if no error
		else
		{
			printf("Press ctrl-c to quit.\n\n");
			//loop forever			while(1)
			{
				//if MIDI input is true				if(Pm_Poll(mstream[0]))
				{
					//read messages to buffer and loop through					cnt = Pm_Read(mstream[0], msg, 32);
										for(i=0; i<cnt; i++)
					{						int chan = 0;
						//if note on
						if((Pm_MessageStatus(msg[i].message)) == MD_NOTEON)						{
							//loop to arpeggiate							for(j=1; j < 5; j++)
							{	
								note = Pm_MessageData1(msg[i].message) + (j*4);
								
								//note on
								Pm_WriteShort(mstream[1], 0, Pm_Message(MD_NOTEON | chan, note, 120));
																time = Pt_Time(NULL);								while(Pt_Time(NULL)  - time < 100); //1/10 second
								
								//note on at velocity of zero to take advantage of running status
								Pm_WriteShort(mstream[1], 0, Pm_Message(MD_NOTEON | chan, note, 0));
							}
						}					}				}				}
						Pm_Close(mstream[0]);  			Pm_Close(mstream[1]);		}
	}
		else printf("No MIDI devices found\n");
	Pm_Terminate();	return 0;}