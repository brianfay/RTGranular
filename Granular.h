#ifndef GRANULAR_H
#define GRANULAR_H

typedef float SAMPLE;
#define SAMPLERATE 44800
#define DELAYLINE_SECONDS 5
#define ENVELOPE_SIZE 1024
#define MAX_NUM_GRAINS 100


typedef enum EnvelopeType{
	COSINE = 0,
	RAMP_UP = 1,
	RAMP_DOWN =  2
} EnvelopeType;

typedef struct Grain{
	int offset;
	int readIndex;
	int direction;
	float speed;
	int samplesRemaining;
	int totalSamples;
	EnvelopeType envelopeType;
} Grain;

typedef enum SchedulingMode{
	SYNCHRONOUS = 0,
	ASYNCHRONOUS = 1
} SchedulingMode;

typedef struct Scheduler{
	SchedulingMode schedulingMode;
} Scheduler;

typedef struct Envelope{
	float cosineTable[ENVELOPE_SIZE];
	float rampUpTable[ENVELOPE_SIZE];
	float rampDownTable[ENVELOPE_SIZE];
} Envelope;

float delayLine[DELAYLINE_SECONDS * SAMPLERATE];
Grain grains[MAX_NUM_GRAINS];
Scheduler scheduler;
Envelope envelope;

//Envelope
void initEnvelope();
float getAmplitude(float percentageComplete, EnvelopeType envType);

//Grain
void initGrain(Grain* grain, int direction, float speed, int msRemaining, int offsetInMs, EnvelopeType envelopeType);
float synthesize(Grain* grain);

//Scheduler
int run();

#endif /* GRANULAR_H */