#include "Granular.h"

void initEnvelope(){
	int i;
	for(i=0; i<ENVELOPE_SIZE; i++){
		//just using ramp ups for now
		envelope.cosineTable[i] = (float)i / (float)ENVELOPE_SIZE;
		envelope.rampUpTable[i] = (float)i / (float)ENVELOPE_SIZE;
		envelope.rampDownTable[i] = (float)i / (float)ENVELOPE_SIZE;
	}
}

float getAmplitude(float percentageComplete, EnvelopeType envelopeType){
	float amp;
	switch(envelopeType){
		case COSINE:
			amp = envelope.cosineTable[(int)(percentageComplete * 0.01 * ENVELOPE_SIZE)];
			break;
		case RAMP_UP:
			amp = envelope.rampUpTable[(int)(percentageComplete * 0.01 * ENVELOPE_SIZE)];
			break;
		case RAMP_DOWN:
			amp = envelope.rampDownTable[(int)(percentageComplete * 0.01 * ENVELOPE_SIZE)];
			break;
		default:
			amp = envelope.cosineTable[(int)(percentageComplete * 0.01 * ENVELOPE_SIZE)];
	}
	return amp;
}
