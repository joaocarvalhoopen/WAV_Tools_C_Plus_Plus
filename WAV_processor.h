#pragma once

#ifndef __WAV_PROCESSOR_H_
#define __WAV_PROCESSOR_H_
 
#include <string>

using namespace std;

//#define _SCL_SECURE_NO_WARNINGS

class WAV_processor {

public:

	enum DSP {
		NONE     = 0,
		ENVELOPE = 1
	};

	enum ELEMENT_TYPE {
		INPUT_FILE      = 0,
		FIRST_PROCESSED = 1,
		LAST_PROCESSED  = 2,
		VECTOR_NUMBER   = 3
	};

	// Construcctor
	WAV_processor(string inputFileName, string inputPath);
	~WAV_processor();

	// Member functions declaration
	bool loadInputFile();
	int process();

	string getErrorMSG();

private:

	bool createOutputFileBuffer(string fileName, WAV_file * & outputWAVFile, int16_t * & outputBuffer16BitsMono );

	string inputFileName;
	string inputPath;
	WAV_file * inputWAVFile;
	int16_t * inputFileBuffer16BitsMono;
	string lastErrorMSG;
	uint32_t numberOfSamples;

};


#endif