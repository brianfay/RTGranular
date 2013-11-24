#include "Granular.h"
#include <stdio.h>

void initGrain(Grain* grain, int direction, float speed, int durationInMs, int offsetInMs, int currentReadPosition, EnvelopeType envelopeType){
	grain->direction = direction;
	grain->speed = speed;
	grain->totalSamples = (int)((float)durationInMs * (float)SAMPLERATE * 0.001);
	//fprintf(stdout, "totalSamples: %d\n", grain->totalSamples);
	grain->samplesRemaining = grain->totalSamples;
	grain->offset = (int)((float)(offsetInMs * SAMPLERATE) * .001);
	grain->readIndex = currentReadPosition + grain->offset;
}

float synthesize(Grain* grain){
	//TODO: lerp
	if(grain->samplesRemaining){
		while(grain->readIndex < 0){
			grain->readIndex += (DELAYLINE_SAMPLES);
		}
		while(grain->readIndex >= (DELAYLINE_SAMPLES)){
			grain->readIndex -= (DELAYLINE_SAMPLES);
		}
		float percentageComplete = (float)(grain->samplesRemaining)/(float)(grain->totalSamples);
		float amp = getAmplitude(percentageComplete,grain->envelopeType);
		//fprintf(stdout, "amplitude = %f\n", amp);
		float out = delayLine[grain->readIndex] * amp;
		grain->readIndex += (1 * grain->direction * grain->speed);
		grain->samplesRemaining--;
		//fprintf(stdout,"Outvalue: %f\n", out);
		return out;
	}
	else{
		return 0;
	}
}
