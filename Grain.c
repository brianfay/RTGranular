#include "Granular.h"
#include <stdio.h>
#include <math.h>

void initGrain(Grain* grain, int direction, float speed, int durationInMs, int offsetInMs, int currentReadPosition, EnvelopeType envelopeType){
	grain->direction = direction;
	grain->speed = speed;
	grain->totalSamples = (int)((float)durationInMs * (float)SAMPLERATE * 0.001);
	//fprintf(stdout, "totalSamples: %d\n", grain->totalSamples);
	grain->samplesRemaining = grain->totalSamples;
	grain->offset = (int)((float)(offsetInMs * SAMPLERATE) * .001);
	grain->readIndex = currentReadPosition + grain->offset;
	grain->envelopeType = envelopeType;
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
		
		//linear interpolation:
		int floorIndex = floorf(grain->readIndex);
		float r = grain->readIndex - floorIndex;
		float n0 = delayLine[floorIndex];
		float n1 = delayLine[(floorIndex + 1) % DELAYLINE_SAMPLES];
		float out = (((1 - r) * n0) + (r * n1)) * amp;

		grain->readIndex += (1 * grain->direction * grain->speed);
		grain->samplesRemaining--;
		//fprintf(stdout,"Outvalue: %f\n", out);
		return out;
	}
	else{
		return 0;
	}
}
