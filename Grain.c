#include "Granular.h"
#include <stdio.h>

void initGrain(Grain* grain, int direction, float speed, int durationInMs, int offsetInMs, EnvelopeType envelopeType){
	grain->direction = direction;
	grain->speed = speed;
	grain->totalSamples = (int)((float)durationInMs * (float)SAMPLERATE * 0.001);
	fprintf(stdout, "totalSamples: %d\n", grain->totalSamples);
	grain->samplesRemaining = grain->totalSamples;
	grain->offset = (int)((float)(offsetInMs * SAMPLERATE) * .001);
	grain->readIndex = grain->offset;
}

float synthesize(Grain* grain){
	//TODO: lerp
	if(grain->samplesRemaining){
		while(grain->readIndex < 0){
			grain->readIndex += (DELAYLINE_SECONDS * SAMPLERATE);
		}
		while(grain->readIndex >= (DELAYLINE_SECONDS * SAMPLERATE)){
			grain->readIndex -= (DELAYLINE_SECONDS * SAMPLERATE);
		}
		float percentageComplete = (float)(grain->samplesRemaining)/(float)(grain->totalSamples);
		float amp = 1;//getAmplitude(percentageComplete,grain->envelopeType);
		float out = delayLine[grain->readIndex] * amp;
		grain->readIndex = grain->readIndex + (1 * grain->direction * grain->speed);
		grain->samplesRemaining--;
		//fprintf(stdout,"Samples Remaining: %d\n", grain->samplesRemaining);
		return out;
	}
	else{
		return 0;
	}
}
