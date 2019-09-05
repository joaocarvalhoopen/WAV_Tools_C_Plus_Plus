#include <iostream>
#include <string>
#include "WAV_file.h"
#include "DSP_envelope.h"
#include "DSP_zeroCrossing.h"
#include "WAV_processor.h"


using namespace std;

//////////////////
// Public methods.	

// Constructor
WAV_processor::WAV_processor(string inputFileName, string inputPath) {
	this->inputFileName = inputFileName;
	this->inputPath = inputPath;
	inputWAVFile = nullptr;
	inputFileBuffer16BitsMono = nullptr;
}

// Destructor
WAV_processor::~WAV_processor() {
}

// Member functions definitions
bool WAV_processor::loadInputFile() {
	inputWAVFile = new WAV_file();
	inputWAVFile->setName(inputFileName);
	inputWAVFile->setPath(inputPath);
	bool error_ocurred = inputWAVFile->loadFile();
	if (error_ocurred) {
		cout << inputWAVFile->getErrorMSG() << endl;
		return true;
	}
	inputWAVFile->printWAVHeader();

	inputWAVFile->getBuffer_16_bits_mono(inputFileBuffer16BitsMono);
	return false;
}

int WAV_processor::process() {

	// TODO: Add the code for the vector of the processed buffers.


	////////////////
	// DSP Envelope.
	////////////////

	string fileName = "output_envelope.wav";
	WAV_file * envelopeOutFile;
	int16_t *  envelopeOutBuffer16BitsMono;
	createOutputFileBuffer(fileName, envelopeOutFile, envelopeOutBuffer16BitsMono);

	// Initialize the algorithm.
	//int sampleRate = 12000;
	int sampleRate = inputWAVFile->getSampleRate();
	int chunk_ms = 32;
	int attack_ms = 5;
	int release_ms = 25;
	DSP_envelope l_dsp_envelope = DSP_envelope(sampleRate, attack_ms, release_ms);

	// Divide into chunks and process each chunk.
	// Collects the output of each chunk.

	//               SampleRate / ms
	// int chunkSize = static_cast<int>(12000 / 0.032);
	int chunkSize = static_cast<int>( ( (sampleRate * 1.0)  / 1000) * chunk_ms );
	uint32_t numberOfChunks = inputWAVFile->getNumberOfSamples() / chunkSize;
	// IMPORTANTE: We are loosing the last chunk it it is incomplete,
	//             if the WAV file isn't a multiple of the chunk_ms.
	for (uint32_t i = 0; i < numberOfChunks; i++) {
		int16_t * inputChunkPtr = inputFileBuffer16BitsMono + (i * chunkSize);
		int16_t * outputChunkPtr = envelopeOutBuffer16BitsMono + (i * chunkSize);
		l_dsp_envelope.processChunk(inputChunkPtr, outputChunkPtr, chunkSize);
	}


	envelopeOutFile->setBuffer_16_bits_mono(envelopeOutBuffer16BitsMono);
	envelopeOutFile->writeFile();
	delete envelopeOutFile;
	envelopeOutFile = nullptr;
	
   /**************************/	
	
	////////////////////
	// DSP zeroCrossing.
	////////////////////

	fileName = "output_zeroCrossing.wav";
	WAV_file * zeroCrossOutFile;
	int16_t *  zeroCrossOutBuffer16BitsMono;
	createOutputFileBuffer(fileName, zeroCrossOutFile, zeroCrossOutBuffer16BitsMono);

	// Initialize the algorithm.
	DSP_zeroCrossing l_dsp_zeroCross = DSP_zeroCrossing(sampleRate);

	// Divide into chunks and process each chunk.
	// Collects the output of each chunk.

	//               SampleRate / ms
	// int chunkSize = static_cast<int>(12000 / 0.032);
	//int chunkSize = static_cast<int>(((sampleRate * 1.0) / 1000) * chunk_ms);
	//uint32_t numberOfChunks = inputWAVFile->getNumberOfSamples() / chunkSize;
	// IMPORTANTE: We are loosing the last chunk it it is incomplete,
	//             if the WAV file isn't a multiple of the chunk_ms.
	for (uint32_t i = 0; i < numberOfChunks; i++) {
		int16_t * inputChunkPtr = inputFileBuffer16BitsMono + (i * chunkSize);
		int16_t * outputChunkPtr = zeroCrossOutBuffer16BitsMono + (i * chunkSize);
		l_dsp_zeroCross.processChunk(inputChunkPtr, outputChunkPtr, chunkSize);
	}


	zeroCrossOutFile->setBuffer_16_bits_mono(zeroCrossOutBuffer16BitsMono);
	zeroCrossOutFile->writeFile();
	delete zeroCrossOutFile;
	zeroCrossOutFile = nullptr;

	

    /*************************/
	
	// Clean up buffers.
	delete[] envelopeOutBuffer16BitsMono;
	envelopeOutBuffer16BitsMono = nullptr;

	delete[] zeroCrossOutBuffer16BitsMono;
	zeroCrossOutBuffer16BitsMono = nullptr;

	return 0;
}

string WAV_processor::getErrorMSG() {
	return lastErrorMSG;
}

///////////////////
// Private methods.

bool WAV_processor::createOutputFileBuffer(string fileName, WAV_file * & outputWAVFile, int16_t * & outputBuffer16BitsMono) {

	// TODO: There should be a copy constructor in the WAV_File class, it would make this code simpler.

	outputWAVFile = new WAV_file();
	outputWAVFile->setName(fileName);
	outputWAVFile->setPath(inputWAVFile->getPath());

	outputWAVFile->setBitsPerSample(inputWAVFile->getBitsPerSample());
	outputWAVFile->setNumChannels(inputWAVFile->getNumChannels());
	outputWAVFile->setSampleRate(inputWAVFile->getSampleRate());
	outputWAVFile->setNumberOfSamples(inputWAVFile->getNumberOfSamples());

	outputWAVFile->initializeWaveHeaderStructBeforeWriting();

	// Allocate the memory for the buffer.
	outputWAVFile->getBuffer_16_bits_mono(outputBuffer16BitsMono);

	return false;
}

