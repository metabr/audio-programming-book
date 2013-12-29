#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include "tinyAudioLib.h"
#include "portsf.h"
#include "threads.h"

Fl_File_Chooser		*fc;
Fl_Thread play_thread;
Fl_Box *texFile;

char filename[256] = {0};

bool isPlaying = false;
bool isStopped = false;

void cb_butFileOpen(void)
{
	static char currDir[256] = {0};
	static int filtVal=0;
	
	fc->filter("Audio Files (*.{wav,aif,aiff})");
	fc->filter_value(filtVal);
	fc->directory(currDir);
	fc->preview(0);
	fc->show();
	while (fc->visible())
		Fl::wait();
	strcpy(currDir,fc->directory());
	filtVal = fc->filter_value();
	if (fc->count() > 0  && currDir[0] != '\0') {// a file is selected and OK is pressed
		strcpy (filename, fc->value());
		texFile->label(filename);
	}
}

#define FRAME_BLOCK_LEN 512

void* play(void *p)
{
	float buf[FRAME_BLOCK_LEN * 2]; // buffer space for stereo (and mono) 
	int sfd; // audio file descriptor 
	int	opened = 0; // flag telling if audio file has been opened 
	PSF_PROPS props; // struct filled by psf_sndOpen(), containing audio file info 
	long nread; // number of frames actually read 
	
	if ( filename[0] == '\0'   ) {  // needs a command line argument 
		fl_message("Choose an audio file prior to play it!");
		return 0;
	}
	
	psf_init(); // initialize Dobson Audio Lib 
	sfd = psf_sndOpen(filename, &props, 0); // open an audio file using Dobson Audio Lib 
	
	if (sfd < 0) { // error condition 
		fl_message("Cannot play audio file.\nBe sure that an audio file is really chosen!");
		goto end;
	}
	
	if (props.chans > 2) {
		fl_message("Invalid number of channels in audio file");
		goto end;
	}
	setOutputLatency(0.2); // from Tyny Audio Lib
	isPlaying = true;
	// ========= ENGINE =========
	do  { 
		nread = psf_sndReadFloatFrames(sfd, buf, FRAME_BLOCK_LEN); 
		if (props.chans == 2)  // stereo 
			outBlockInterleaved(buf, FRAME_BLOCK_LEN);
		else  // mono
			outBlockMono(buf, FRAME_BLOCK_LEN);
	} while (nread == FRAME_BLOCK_LEN && !isStopped);
	// ======== ENGINE END ======
end:
	
	psf_sndClose(sfd);
	psf_finish();
	isPlaying = false;
	isStopped = false;
	return 0;
}

void cb_butPlay(Fl_Button*, void*) {
	if(!isPlaying)
		fl_create_thread(play_thread, play, NULL);
	else 
		fl_message("Still playing current audio file. Wait!");
}

void cb_butStop(Fl_Button*, void*) {
	if (isPlaying) isStopped = true;
}

int main()
{
	// ---------- GUI DEFINITION ----------
	Fl_Window *MainWindow = new Fl_Window(335, 147, "Simple audio player");
	Fl_Button* butFileOpen = new Fl_Button(60, 45, 210, 25, "Choose Audio File");
	butFileOpen->callback((Fl_Callback*)cb_butFileOpen);
	texFile = new Fl_Box(0, 20, 335, 20, "Choose a file to play, please...");
	Fl_Button* butPlay = new Fl_Button(60, 85, 60, 50, "Play");
	butPlay->callback((Fl_Callback*)cb_butPlay);
	Fl_Button* butStop = new Fl_Button(210, 85, 60, 50, "Stop");
	butStop->callback((Fl_Callback*)cb_butStop);
	MainWindow->end();
	
	fc = new Fl_File_Chooser(".", "*", Fl_File_Chooser::SINGLE, "Open Audio File");
	MainWindow->show();
	return Fl::run();
	// ----- end of GUI -------------------
}


