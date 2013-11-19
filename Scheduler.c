#include "Granular.h"
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>

int readIndex;
int samplesPassed;

int audioCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData );

int audioCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData ){
	SAMPLE *out = (SAMPLE*)outputBuffer;
	const SAMPLE *in = (const SAMPLE*)inputBuffer;
	unsigned int i;
	(void) timeInfo;
	(void) statusFlags;
		for(i=0; i<framesPerBuffer; i++)
		{
			int j;
			//one grain scheduled per second
			if(samplesPassed % (SAMPLERATE) == 1){
				for(j = 0; j < MAX_NUM_GRAINS; j++){
					//fprintf(stdout, "grains being initialized!\n");
					if(!grains[j].samplesRemaining){
						int direction;
						float speed = 2;
						int durationInMs = 400;
						int offsetInMs = 200;
						if((double)rand()/RAND_MAX > 0.5){
							direction = -1;
						} else{
							direction = 1;
						}
						initGrain(&(grains[j]), direction, speed, durationInMs, offsetInMs, COSINE);
						break;
					}
				}
			}
			//copied twice for the two channels
			int k;
			for(k=0; k<2; k++){
				readIndex %= (DELAYLINE_SECONDS * SAMPLERATE);
				*out = *in++;
				delayLine[readIndex++] = *out;
				for(j = 0; j<MAX_NUM_GRAINS; j++){
					*out += synthesize(&grains[j]);
				}
				*out++;
			}
			samplesPassed++;
	}
	return paContinue;		
}

int run(){
	srand(time(NULL));
	PaError err;
	err = Pa_Initialize();
	if(err != paNoError) goto error;
	int numInputChannels, numOutputChannels;
	int inputDeviceIndex = 7, outputDeviceIndex = 6;
	PaStreamParameters inputParameters, outputParameters;
	inputParameters.device = inputDeviceIndex;
	outputParameters.device = outputDeviceIndex;
	const PaDeviceInfo* inputDeviceInfo = Pa_GetDeviceInfo(inputDeviceIndex);
	const PaDeviceInfo* outputDeviceInfo = Pa_GetDeviceInfo(outputDeviceIndex);
	inputParameters.channelCount = inputDeviceInfo->maxInputChannels;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = inputDeviceInfo->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	outputParameters.device = outputDeviceIndex;
	if(outputParameters.device == paNoDevice){
		fprintf(stderr,"Error: Input device is invalid.\n Maybe the device has been unplugged?");
		goto error;
	}
	if(outputParameters.device == paNoDevice){
		fprintf(stderr,"Error: Output device is invalid.\n Maybe the device has been unplugged?");
		goto error;
	}
	outputParameters.channelCount = outputDeviceInfo->maxOutputChannels;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = outputDeviceInfo->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	
	numInputChannels = inputParameters.channelCount;
	numOutputChannels = outputParameters.channelCount;
	fprintf(stderr,"numInputChannels: %d \n", numInputChannels);
	fprintf(stderr,"numOutputChannels: %d \n", numOutputChannels);
	
	readIndex = 0;
	samplesPassed = 0;
	PaStream* stream;
	err = Pa_OpenStream(
			&stream,
			&inputParameters,
			&outputParameters,
			inputDeviceInfo->defaultSampleRate,	
			paFramesPerBufferUnspecified,
			paClipOff,
			audioCallback,
			NULL);
	
	fprintf(stderr, "Error: %d\n", err);
	if(err != paNoError) goto error;
	err = Pa_StartStream(stream);
	
	//pause application
	while(1){};
	
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	err = Pa_Terminate();
	if( err != paNoError ) goto error;
	return 0;
	error:
	fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    err = Pa_Terminate();
    return -1;
}
