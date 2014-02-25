#include <stdio.h>
#include <math.h>

#include <portaudio.h>

#define FRAME_BLOCK_LEN 512
#define SAMPLING_RATE 44100

PaStream *audioStream;
double si = 0;

int audio_callback(const void *input_buf, void *output_buf,
		   unsigned long framesPerBuffer,
		   const PaStreamCallbackTimeInfo *timeInfo,
		   PaStreamCallbackFlags statusFlag,
		   void *userData)
{
	float *in = (float*) input_buf;
	float *out = (float*) output_buf;
	static double phase = 0;
	for (unsigned long i = 0; i < framesPerBuffer; i++) {
		float sine = sin(phase);
		*out++ = *in++ * sine; // left channel
		*out++ = *in++ * sine; // right channel
		phase += si;
	}
	return paContinue;
}

void printOutputDevices()
{
	const PaDeviceInfo *info;
	const PaHostApiInfo *hostApi;

	for(int i = 0; i < Pa_GetDeviceCount(); i++) {
		info = Pa_GetDeviceInfo(i);
		hostApi = Pa_GetHostApiInfo(info->hostApi);
		if (info->maxOutputChannels > 0)
			printf("%d: [%s] %s (output)\n", i, hostApi->name, info->name);
	}
}

void printInputDevices()
{
	const PaDeviceInfo *info;
	const PaHostApiInfo *hostApi;

	for (int i = 0; i < Pa_GetDeviceCount(); i++) {
		info = Pa_GetDeviceInfo(i);
		hostApi = Pa_GetHostApiInfo(info->hostApi);
		if (info->maxInputChannels > 0)
			printf("%d: [%s] %s (input)\n", i, hostApi->name, info->name);
	}
}
void init()
{
	int id;
	float frequency;
	const PaDeviceInfo *info;
	const PaHostApiInfo *hostApi;
	PaStreamParameters inputParams, outputParams;

	printf("Modulator frequency (Hz): ");
	scanf("%f", &frequency);

	si = 2 * M_PI * frequency / SAMPLING_RATE;

	printf("Initializing Portaudio...\n");
	Pa_Initialize();

	printOutputDevices();

	printf("Output device number: ");
	scanf("%d", &id);

	info = Pa_GetDeviceInfo(id);
	hostApi = Pa_GetHostApiInfo(info->hostApi);

	printf("Opening audio output device [%s] %s\n", hostApi->name, info->name);

	outputParams.device = id;
	outputParams.channelCount = 2;
	outputParams.sampleFormat = paFloat32;
	outputParams.suggestedLatency = info->defaultLowOutputLatency;
	outputParams.hostApiSpecificStreamInfo = NULL;

	printInputDevices();

	printf("Input device number: ");
	scanf("%d", &id);

	info = Pa_GetDeviceInfo(id);
	hostApi = Pa_GetHostApiInfo(info->hostApi);

	printf("Opening audio input device [%s] %s\n", hostApi->name, info->name);

	inputParams.device = id;
	inputParams.channelCount = 2;
	inputParams.sampleFormat = paFloat32;
	inputParams.suggestedLatency = info->defaultLowInputLatency;
	inputParams.hostApiSpecificStreamInfo = NULL;

	Pa_OpenStream(&audioStream, &inputParams, &outputParams, SAMPLING_RATE,
			FRAME_BLOCK_LEN, paClipOff, audio_callback, NULL);
	Pa_StartStream(audioStream);
}

void terminate()
{
	Pa_StopStream(audioStream);
	Pa_CloseStream(audioStream);
	Pa_Terminate();
}

int main(int argc, char **argv)
{
	init();

	printf("running...press space bar and return to exit\n");

	while(getchar() != ' ')
		Pa_Sleep(100);

	terminate();
	return 0;
}
