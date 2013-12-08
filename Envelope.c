#include "Granular.h"
#include <math.h>

void initEnvelope(){
	int i;
	for(i=0; i<ENVELOPE_SIZE; i++){
		//just using ramp ups for now
		envelope.cosineTable[i] = 0.5 * (1 - cos(((TWO_PI*i) / ((float)ENVELOPE_SIZE - 1))));
		envelope.rampUpTable[i] = (float)i / (float)ENVELOPE_SIZE;
		envelope.rampDownTable[i] = (float)i / (float)ENVELOPE_SIZE;
	}
}

float getAmplitude(float percentageComplete, EnvelopeType envelopeType){
	float amp;
	switch(envelopeType){
		case HANNING:
			amp = envelope.cosineTable[(int)(percentageComplete * ENVELOPE_SIZE)];
			break;
		case RAMP_UP:
			amp = envelope.rampUpTable[(int)(percentageComplete * ENVELOPE_SIZE)];
			break;
		case RAMP_DOWN:
			amp = envelope.rampDownTable[(int)(percentageComplete * ENVELOPE_SIZE)];
			break;
		default:
			amp = envelope.cosineTable[(int)(percentageComplete * ENVELOPE_SIZE)];
	}
	return amp;
}
