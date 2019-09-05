#pragma once

#ifndef __DSP_ENVELOPE_H_
#define __DSP_ENVELOPE_H_

#include <string>

using namespace std;

class DSP_envelope {

public:

	// Construcctor
	DSP_envelope(int sampleRate, int attack_ms, int release_ms);
	~DSP_envelope();

	// Member functions declaration
	void processChunk(int16_t * & inBuffer_16_bits, int16_t * & outBuffer_16_bits, int chunkSize);
	
private:
	int16_t processSample(int16_t sample_N);

	int sampleRate;
	int chunk_ms;
	int attack_ms;
	int release_ms;
	double attack_coef;
	double release_coef;
	int16_t currentState;
	string lastErrorMSG;
};

#endif
