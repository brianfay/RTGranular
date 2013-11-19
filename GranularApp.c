#include "Granular.h"

int main(){
	//initialize grain array, delayline, envelope, scheduler, set everything into motion
	int i; 
	initEnvelope();
	for(i = 0; i< MAX_NUM_GRAINS; i++){
		//init all grains to dead so that they must be scheduled before doing anything
		(grains[i]).samplesRemaining = 0;
	}
	for(i = 0; i < (DELAYLINE_SECONDS * SAMPLERATE); i++){
		delayLine[i] = 0.0;
	}
	scheduler.schedulingMode = ASYNCHRONOUS;
	int err = run();
	return err;
}
