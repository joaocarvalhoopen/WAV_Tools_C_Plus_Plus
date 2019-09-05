#pragma once


#ifndef __WAV_FILE_H_
#define __WAV_FILE_H_

#include <string>

using namespace std;

//#define _SCL_SECURE_NO_WARNINGS

class WAV_file{

public:

	enum NUM_CHANNELS { 
		ONE = 1,
		TWO = 2 
	};
	
	enum BITS_PER_SAMPLE { 
		BPS_8_BITS = 8,
		BPS_16_BITS = 16 
	};

	typedef struct {
		char     chunk_id[4];
		uint32_t chunk_size;
		char     format[4];
		char     fmtchunk_id[4];
		uint32_t fmtchunk_size;
		uint16_t audio_format;
		uint16_t num_channels;
		uint32_t sample_rate;
		uint32_t byte_rate;
		uint16_t block_align;
		uint16_t bps;
		char     datachunk_id[4];
		uint32_t datachunk_size;
	}WavHeader;

	// Construcctor
	WAV_file();

	~WAV_file();
	
    // Member functions declaration 
	void setName(string name);
	string getName(void);
	void setPath(string path);
	string getPath(void);

	void setNumChannels(NUM_CHANNELS numChannels);
	NUM_CHANNELS getNumChannels();
	void setSampleRate(uint32_t sampleRate);
	uint32_t getSampleRate();
	void setByteRate(uint32_t byteRate);
	uint32_t getByteRate();
	void setBitsPerSample(BITS_PER_SAMPLE bitsPerSample);
	BITS_PER_SAMPLE getBitsPerSample();

	void setNumberOfSamples(uint32_t numberOfSamples);
	uint32_t getNumberOfSamples();

	bool isStereo();

	bool initializeWaveHeaderStructBeforeWriting();

	void printWAVHeader();
	
	bool loadFile();
	bool writeFile();

	string getErrorMSG();

	int getBuffer_8_bits_mono(uint8_t* & newBuffer_1);
	int getBuffer_8_bits_stereo(uint8_t* & newBuffer_1_left, uint8_t* & newBuffer_2_right);

	int getBuffer_16_bits_mono(int16_t* & newBuffer_1);
	int getBuffer_16_bits_stereo(int16_t* & newBuffer_1_left, int16_t* & newBuffer_2_right);

	int setBuffer_8_bits_mono(uint8_t* & newBuffer_1);
	int setBuffer_8_bits_stereo(uint8_t* & newBuffer_1_left, uint8_t* & newBuffer_2_right);

	int setBuffer_16_bits_mono(int16_t* & newBuffer_1);
	int setBuffer_16_bits_stereo(int16_t* & newBuffer_1_left, int16_t* & newBuffer_2_right);


private:
	string name;
	string path;
	
	WavHeader waveHeader;

	string lastErrorMSG;

	uint32_t numberOfSamples;

	uint8_t * bufferInternal_uint8_t = nullptr;
	int16_t * bufferInternal_int16_t = nullptr;
};



#endif
