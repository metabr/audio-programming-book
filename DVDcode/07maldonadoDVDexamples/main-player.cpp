#include <stdio.h>
#include <FL/x.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>
#include "PlayerGUI.h"
#include "tinyAudioLib.h"
#include "portsf.h"
#include "threads.h"
Fl_Thread play_thread;

#define FRAME_BLOCK_LEN 512

void load(void *gui)
{
	static char currDir[256] = {0};
	static int filtVal=0;
	PlayerGUI *p = (PlayerGUI*) gui;
	p->fc->type(0); // only single files
	p->fc->filter("Audio Files (*.{wav,aif,aiff})");
	p->fc->filter_value(filtVal);
	p->fc->directory(currDir);
	p->fc->value("");
	p->fc->preview(0); // no preview
	p->fc->show();
	while (p->fc->visible())
		Fl::wait();
	strcpy(currDir,p->fc->directory());
	filtVal = p->fc->filter_value();
	if (p->fc->count() > 0 && currDir[0] != '\0') { // a file is selected and OK is pressed
		strcpy (p->filename, p->fc->value());
		p->texFile->label(p->filename);
	
		int sfd;
		PSF_PROPS props; // struct filled by psf_sndOpen(), containing audio file info WAVE_FILE
		sfd = psf_sndOpen(p->filename, &props, 0);
		double length = (double)  psf_sndSize(sfd) / (double) props.srate;
		p->sldProgressBar->range(0, length);
		p->txtStart->range(0, length);
		p->ffStep = length/20;
		psf_sndClose(sfd);
	}
}


void* playImp(void *gui)
{
	PlayerGUI *p = (PlayerGUI*) gui;
	float buf[FRAME_BLOCK_LEN * 2]; // buffer space for stereo (and mono) WAVE_FILE
	int sfd; // audio file descriptor WAVE_FILE
	int	opened = 0; // flag telling if audio file has been opened WAVE_FILE
	PSF_PROPS props; // struct filled by psf_sndOpen(), containing audio file info WAVE_FILE
	long counter; // counter of frames read WAVE_FILE
	long length;	// length of file in frames WAVE_FILE
	long endpoint; // end point of playing WAVE_FILE
	
	long nread; // number of frames actually read WAVE_FILE
	double startime, dur;
	
	if ( p->filename[0] == '\0'   ) {  // needs a command line argument 
		fl_message("Choose an audio file prior to play it!");
		return 0;
	}
	
	startime = p->txtStart->value();
	dur = p->txtDur->value();
	#ifdef WIN32
		HWND callback_target = fl_xid(p->MainWindow);
	#endif

	psf_init(); // initialize Dobson Audio Lib WAVE_FILE
	sfd = psf_sndOpen(p->filename, &props, 0); // open an audio file using Dobson Audio Lib WAVE_FILE
	if (sfd < 0) { // error condition WAVE_FILE
		printf("An error occurred opening audio file\n");
		goto end;
	}
	
	printf("file \'%s\' opened...\n", p->filename);
	printf("sampling rate: %d\n", props.srate);
	printf("number of chans: %d\n", props.chans);
	length = psf_sndSize(sfd);
	p->sldProgressBar->range(0, (double) length / (double) props.srate);
	if (startime > 0) 
		counter = (long) (startime * props.srate); // length in frames WAVE_FILE
	else 
		counter = 0; // beginning of file WAVE_FILE
	
	if (dur > 0) {
		endpoint = (long) (dur  * props.srate + counter);
		endpoint = (endpoint < length) ? endpoint : length;
	}
	else {
		endpoint = length; 
		dur = (double) (endpoint-counter) / (double) props.srate;
	}
	
	if (props.chans > 2) {
		fl_message("Invalid number of channels in audio file, max 2 chans allowed");
		goto end;
	}
	
	psf_sndSeek(sfd, counter, PSF_SEEK_SET); // begin position at the appropriate point WAVE_FILE
	setOutputLatency(0.2); // from Tyny Audio Lib
	p->isPlaying = true;
	// ========= ENGINE =========
	do {
		double pos = (double)counter / (double) props.srate;
		p->sldProgressBar->value(pos); 
		p->txtPos->value(pos);
		#ifdef WIN32
			PostMessage(callback_target,0,0,0);
		#endif
		nread = psf_sndReadFloatFrames(sfd, buf, FRAME_BLOCK_LEN); 
		if (props.chans == 2)  // stereo WAVE_FILE
			outBlockInterleaved(buf, FRAME_BLOCK_LEN);
		else  // mono WAVE_FILE
			outBlockMono(buf, FRAME_BLOCK_LEN);
		counter+=FRAME_BLOCK_LEN;
	} while (counter < endpoint && !p->isStopped);
	// ======== ENGINE END ======
end:
	printf("finished!\n");
	psf_sndClose(sfd);
	psf_finish();
	p->isPlaying = false;
	p->isStopped = false;
	return 0;
}


void play(void *gui)
{
	PlayerGUI *p = (PlayerGUI*) gui;
	if(!p->isPlaying)
		fl_create_thread(play_thread, playImp, gui);
	else 
		fl_message("Still playing current audio file. Wait!");
}

void stop(void *gui)
{
	PlayerGUI *p = (PlayerGUI*) gui;
	if (p->isPlaying) p->isStopped=true;
}

void rew(void *gui)
{	
	PlayerGUI *p = (PlayerGUI*) gui;
	if (p->isPlaying) p->isStopped=true;
	double pos = p->txtPos->value();
	pos -= p->ffStep;
	if (pos < 0) pos = 0;
	p->txtStart->value(pos);
	p->sldProgressBar->value(pos); 
	p->txtPos->value(pos);
}

void ff(void *gui)
{
	PlayerGUI *p = (PlayerGUI*) gui;
	if (p->isPlaying) p->isStopped=true;
	double pos = p->txtPos->value();
	pos += p->ffStep;
	if (pos > p->sldProgressBar->maximum()) pos = p->sldProgressBar->maximum();
	p->txtStart->value(pos);
	p->sldProgressBar->value(pos); 
	p->txtPos->value(pos);
}

void setpos(void *gui, VALUATOR val)
{
	PlayerGUI *p = (PlayerGUI*) gui;
	double pos;
	if (p->isPlaying) p->isStopped=true;
	switch(val) {
	case SLID:
		pos = p->sldProgressBar->value();
		p->txtStart->value(pos);
		break;
	case TXTSTART:
		pos = p->txtStart->value();
		p->sldProgressBar->value(pos);
		break;
	}
	if (pos > p->sldProgressBar->maximum()) pos = p->sldProgressBar->maximum();
	p->txtPos->value(pos);
}

int main()
{
	PlayerGUI *gui = new PlayerGUI;
	gui->show();
	return Fl::run();
}
