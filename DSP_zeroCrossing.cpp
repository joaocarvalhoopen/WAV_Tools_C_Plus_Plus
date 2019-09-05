#include <string>
#include <cmath>
#include "DSP_zeroCrossing.h"

using namespace std;

//////////////////
// Public methods.

DSP_zeroCrossing::DSP_zeroCrossing(int sampleRate) {
	this->sampleRate = sampleRate;
	lastErrorMSG = "";
}

DSP_zeroCrossing::~DSP_zeroCrossing() {
}

void DSP_zeroCrossing::processChunk(int16_t * & inBuffer_16_bits, int16_t * & outBuffer_16_bits, int chunkSize) {
	int counter = 0;
	// Initializes to the first sample.
	int lastValue = inBuffer_16_bits[0];
	// Counts the number of zeroCrossing.
	for (int i = 0; i < chunkSize; i++) {
		if ((inBuffer_16_bits[i] * lastValue) < 0 ) {
			counter++;
		}
		if (inBuffer_16_bits[i] != 0) {
			// Only updates the last value if the value is different from zero.
			lastValue = inBuffer_16_bits[i];
		}
	}
	// Calculate the rate of crossings per second.
	int16_t numberOfChunks = sampleRate / chunkSize;
	int16_t rateOfCrossings = counter * numberOfChunks;
	// Write the chuck medium value to each sample of the output of the chunk.
	for (int i = 0; i < chunkSize; i++) {
		outBuffer_16_bits[i] = rateOfCrossings;
	}
}

///////////////////
// Private methods.
