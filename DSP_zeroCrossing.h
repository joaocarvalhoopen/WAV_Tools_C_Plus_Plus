#pragma once

#ifndef __DSP_ZEROCROSSING_H_
#define __DSP_ZEROCROSSING_H_

#include <string>

using namespace std;

class DSP_zeroCrossing {

public:
	// Construcctor
	DSP_zeroCrossing(int sampleRate);
	~DSP_zeroCrossing();

	// Member functions declaration
	void processChunk(int16_t * & inBuffer_16_bits, int16_t * & outBuffer_16_bits, int chunkSize);

private:
	int sampleRate;
	int chunk_ms;
	string lastErrorMSG;
};

#endif