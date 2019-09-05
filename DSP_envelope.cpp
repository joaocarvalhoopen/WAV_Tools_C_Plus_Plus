
#include <string>
#include <cmath>
#include "DSP_envelope.h"

using namespace std;

//////////////////
// Public methods.

DSP_envelope::DSP_envelope(int sampleRate, int attack_ms, int release_ms) {
	this->sampleRate = sampleRate;
	this->attack_ms = attack_ms; 
	this->release_ms = release_ms;
	// Calculate attack_coefficient.
	attack_coef = exp(-1000.0 / (attack_ms * sampleRate));
	// Calculate release_coefficient.
	release_coef = exp(-1000.0 / (release_ms * sampleRate));
	lastErrorMSG = "";
	currentState = 0;
}

DSP_envelope::~DSP_envelope() {
}

void DSP_envelope::processChunk(int16_t * & inBuffer_16_bits, int16_t * & outBuffer_16_bits, int chunkSize) {
	for (int i = 0; i < chunkSize; i++) {
		outBuffer_16_bits[i] = processSample(inBuffer_16_bits[i]);
	}

}

///////////////////
// Private methods.

inline int16_t DSP_envelope::processSample(int16_t sample_N) {
	if (sample_N > currentState) {
		// Attack mode.
		currentState =  (int16_t) (sample_N + attack_coef * (currentState - sample_N ));
	} else {
		// Release mode.
		currentState = (int16_t) (sample_N + release_coef * (currentState - sample_N ));
	}
	return currentState;
}