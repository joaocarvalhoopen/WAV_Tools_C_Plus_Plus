
#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <new>
#include "WAV_file.h"

 
using namespace std;

// Constructor
WAV_file::WAV_file() {
	bufferInternal_uint8_t = nullptr;
	bufferInternal_int16_t = nullptr;
}

// Destructor
WAV_file::~WAV_file() {
	if (bufferInternal_uint8_t != nullptr) {
		delete[] bufferInternal_uint8_t;
		bufferInternal_uint8_t = nullptr;
	}
	if (bufferInternal_int16_t != nullptr) {
		delete[] bufferInternal_int16_t;
		bufferInternal_int16_t = nullptr;
	}
}

// Member functions definitions
void WAV_file::setName(string name) {
	this->name = name;
}

string WAV_file::getName() {
	return name;
}

void WAV_file::setPath(string path) {
	this->path = path;
}

string WAV_file::getPath() {
	return path;
}

void WAV_file::setNumChannels(WAV_file::NUM_CHANNELS numChannels) {
	this->waveHeader.num_channels = numChannels;
}

WAV_file::NUM_CHANNELS WAV_file::getNumChannels() {
	return static_cast<NUM_CHANNELS>(waveHeader.num_channels);
}

void WAV_file::setSampleRate(uint32_t sampleRate) {
	this->waveHeader.sample_rate = sampleRate;
}

uint32_t WAV_file::getSampleRate() {
	return waveHeader.sample_rate;
}

void WAV_file::setByteRate(uint32_t byteRate) {
	this->waveHeader.byte_rate = byteRate;
}

uint32_t WAV_file::getByteRate() {
	return waveHeader.byte_rate;
}

void WAV_file::setBitsPerSample(BITS_PER_SAMPLE bitsPerSample) {
	this->waveHeader.bps = bitsPerSample;
}

WAV_file::BITS_PER_SAMPLE WAV_file::getBitsPerSample() {
	return static_cast<WAV_file::BITS_PER_SAMPLE>(waveHeader.bps);
}

void WAV_file::setNumberOfSamples(uint32_t numberOfSamples) {
	this->numberOfSamples = numberOfSamples;
}

uint32_t WAV_file::getNumberOfSamples() {
	return numberOfSamples;
}

bool WAV_file::isStereo(){
	if (waveHeader.num_channels == 2)
		return true;
	else
		return false;
}

bool WAV_file::initializeWaveHeaderStructBeforeWriting() {
	// Initial fill of waveHeader data structure.
	memcpy(waveHeader.chunk_id, "RIFF", 4);
	// waveHeader.chunk_size = ;
	memcpy(waveHeader.format, "WAVE", 4);

	memcpy(waveHeader.fmtchunk_id, "fmt ", 4);
	waveHeader.fmtchunk_size = 16;
	waveHeader.audio_format =  1;
	// num_channels;
	if (   waveHeader.num_channels != WAV_file::ONE
		&& waveHeader.num_channels != WAV_file::TWO){
		lastErrorMSG = "ERROR: The number of channels wasn't set to 1 or 2 ";
		return true;
	}
	// sample_rate;
	if (waveHeader.sample_rate == 0) {
		lastErrorMSG = "ERROR: The sample rate wasn't set to ex: 8000 S/s or ex:44100";
		return true;
	}
	waveHeader.byte_rate =   waveHeader.sample_rate 
		                   * waveHeader.num_channels 
		                   * (waveHeader.bps / 8);
	waveHeader.block_align =   waveHeader.num_channels 
		                     * (waveHeader.bps/8 );
	// bps;   // BitsPerSample
	if (   waveHeader.bps != WAV_file::BPS_8_BITS
		&& waveHeader.bps != WAV_file::BPS_16_BITS) {
		lastErrorMSG = "ERROR: The bits per sample wasn't set to 8 or 16 Bits ";
		return true;
	}

	// Fill the data subchunk.
	memcpy(waveHeader.datachunk_id, "data", 4);
	waveHeader.datachunk_size = numberOfSamples * waveHeader.num_channels * (waveHeader.bps / 8);

	// Fill of the the first chunk size. It has to be made out of order.
	waveHeader.chunk_size = 36 + waveHeader.datachunk_size;
	return false; // No error.
}

void WAV_file::printWAVHeader() {
	cout << "################";
	cout << "WAV Header dump." << endl << endl;

    cout << "################" << endl;
	cout << ">> RIFF header." << endl;

	char str_tmp[5];
	copy(begin(waveHeader.chunk_id),
		 end(waveHeader.chunk_id),
		 begin(str_tmp));
	str_tmp[4] = '\0';
	cout << "chunk_id [char 4]:\t" << str_tmp << endl;
	
	cout << "chunk_size [uint32_t]:\t" << this->waveHeader.chunk_size << endl;

	// Initialize the char array buffer.
	std::memset(&str_tmp, 0, sizeof(str_tmp));
	copy(begin(waveHeader.format),
		 end(waveHeader.format),
		 begin(str_tmp));
	str_tmp[4] = '\0';
	cout << "format [char 4]:\t" << str_tmp << endl;

	cout << endl;
	cout << ">> fmt header." << endl;
	// Initialize the char array buffer.
	memset(&str_tmp, 0, sizeof(str_tmp));
	copy(begin(waveHeader.fmtchunk_id),
		 end(waveHeader.fmtchunk_id),
		 begin(str_tmp));
	str_tmp[4] = '\0';
	cout << "fmtchunk_id [char 4]:      " << str_tmp                  << endl;
	cout << "fmtchunk_size [uint32_t]:  " << waveHeader.fmtchunk_size << endl;
	cout << "audio_format [uint16_t]:   " << waveHeader.audio_format  << endl;
	cout << "num_channels [uint16_t]:   " << waveHeader.num_channels  << endl;
	cout << "sample_rate [uint32_t]:    " << waveHeader.sample_rate   << endl;
	cout << "byte_rate [uint32_t]:      " << waveHeader.byte_rate     << endl;
	cout << "block_align [uint16_t]:    " << waveHeader.block_align   << endl;
	cout << "bps [uint16_t]:            " << waveHeader.bps           << endl;

	cout << endl;
	cout << ">> data header." << endl;
	// Initialize the char array buffer.
	memset(&str_tmp, 0, sizeof(str_tmp));
	copy(begin(waveHeader.datachunk_id),
		 end(waveHeader.datachunk_id),
		 begin(str_tmp));
	str_tmp[4] = '\0';
	cout << "datachunk_id [char 4]:\t" << str_tmp << endl;
	cout << "datachunk_size [uint32_t]:\t" << waveHeader.datachunk_size << endl;
}

bool WAV_file::loadFile() {
	fstream fh;
	string completePath = path + name;

	// TODO: Verify that the name of the file ends with .WAV ou .wav, convert to upper case before comparision.

	fh.open(completePath, fstream::in | fstream::binary);
	if (!fh.is_open()) {
		// Error opening the file.
		lastErrorMSG = "ERROR opening file: " + completePath;
		return true;
	}

	// Get length of file.
	fh.seekg(0, fh.end);
	fstream::pos_type length = fh.tellg();
	fh.seekg(0, fh.beg);
	unsigned int wav_struct_size = sizeof(waveHeader);
	if (length < wav_struct_size) {
		// File is smalller then the header of a WAV file.
		lastErrorMSG = "ERROR: File " + completePath + " is smaller then the header of a WAV file";
		fh.close();
		return true;
	}

	// Reads all the structure of the header.
	fh.read((char*)&waveHeader, sizeof(waveHeader));

	// File type validations.
	if (strncmp(waveHeader.chunk_id, "RIFF", 4) ||
		strncmp(waveHeader.format, "WAVE", 4)) {
		lastErrorMSG = "ERROR: File " + completePath + " is not a WAV file";
		fh.close();
		return true;
	}
	if (waveHeader.audio_format != 1) {
		lastErrorMSG = "ERROR: File " + completePath + " the API only supports PCM format in WAV.";
		fh.close();
		return true;
	}

	// Reads the data part of the file.
	switch (waveHeader.bps)
	{

		case WAV_file::BPS_8_BITS:
		{
			if (bufferInternal_uint8_t != nullptr) {
				delete[] bufferInternal_uint8_t;
				bufferInternal_uint8_t = nullptr;
			}
			bufferInternal_uint8_t = new (std::nothrow) uint8_t[waveHeader.datachunk_size];
			if (bufferInternal_uint8_t == nullptr) {
				lastErrorMSG = "Error allocating memory";
				fh.close();
				return true;
			}
			// TODO: Validate if the length of the file isn't truncated.

			// Reads all the structure of the header.
			fh.read((char*)bufferInternal_uint8_t, waveHeader.datachunk_size);
			numberOfSamples = (waveHeader.datachunk_size) / waveHeader.num_channels;
			break;
		}

		case WAV_file::BPS_16_BITS:
		{
			if (bufferInternal_int16_t != nullptr) {
				delete[] bufferInternal_int16_t;
				bufferInternal_int16_t = nullptr;
			}
			bufferInternal_int16_t = new (std::nothrow) int16_t[waveHeader.datachunk_size / sizeof(int16_t)];
			if (bufferInternal_int16_t == nullptr) {
				lastErrorMSG = "Error allocating memory";
				fh.close();
				return true;
			}
			// TODO: Validate if the length of the file isn't truncated.

			// Reads all the structure of the header.
			fh.read((char*)bufferInternal_int16_t, waveHeader.datachunk_size);
			numberOfSamples = (waveHeader.datachunk_size / 2) / waveHeader.num_channels;
			break;
		}

		default:
		{
			break;
		}
	}

	fh.close();
	return false;	
}

bool WAV_file::writeFile() {
	fstream fh;
	string completePath = path + name;

	// TODO: Verify if the name of the file ends with .WAV ou .wav, convert to upper case before comparison.

	fh.open(completePath, fstream::out | fstream::binary | fstream::trunc);
	if (!fh.is_open()) {
		// Error opening the file.
		lastErrorMSG = "ERROR opening file for writing: " + completePath;
		return true;
	}

	unsigned int wav_struct_size = sizeof(waveHeader);
	// Write all the structure of the header.
	fh.write((char*)&waveHeader, sizeof(waveHeader));
	
	// Writes the data part of the file.
	switch (this->waveHeader.bps) 
	{
		case WAV_file::BPS_8_BITS: 
		{
			if (bufferInternal_uint8_t == nullptr) {
				lastErrorMSG = "ERROR: Data buffer uint8_t is NULL!";
				fh.close();
				return true;
			}
			// Writes all the structure of the header.
			fh.write((char*)bufferInternal_uint8_t, waveHeader.datachunk_size);
			break;
		}

		case WAV_file::BPS_16_BITS:
		{
			if (bufferInternal_int16_t == nullptr) {
				lastErrorMSG = "ERROR: Data buffer int16_t is NULL!";
				fh.close();
				return true;
			}
			// Writes all the structure of the header.
			fh.write((char*)bufferInternal_int16_t, waveHeader.datachunk_size);
			break;
		}

		default:
		{
			break;
		}
	}

	fh.close();
	return false;
}

string WAV_file::getErrorMSG() {
	return lastErrorMSG;
}

/////////////////
// Get Buffers
////////////////

// NOTE: If no file have been reade it return an allocated and initialized buffer.
int WAV_file::getBuffer_8_bits_mono(uint8_t* & newBuffer_1) {
	newBuffer_1 = nullptr;
	if (waveHeader.num_channels == WAV_file::NUM_CHANNELS::TWO){
		lastErrorMSG = "ERROR: getBuffer_8_bits_mono() can't be used for a stereo WAV file!";
		return -1;
	}
	newBuffer_1 = new (nothrow) uint8_t[waveHeader.datachunk_size];
	if (newBuffer_1 == nullptr) {
		lastErrorMSG = "Error allocating memory";
		return -1;
	}
	if (bufferInternal_uint8_t != nullptr) {
		copy(bufferInternal_uint8_t,
			bufferInternal_uint8_t + waveHeader.datachunk_size,
			newBuffer_1);
	} else {
		// Initializes the buffer the center value.
		memset(newBuffer_1, _UI8_MAX / 2, waveHeader.datachunk_size);
	}
	return waveHeader.datachunk_size;
}

// NOTE: If no file have been reade it return an allocated and initialized buffer.
int WAV_file::getBuffer_8_bits_stereo(uint8_t* & newBuffer_1_left, uint8_t* & newBuffer_2_right) {
	newBuffer_1_left = nullptr;
	newBuffer_2_right = nullptr;
	if (waveHeader.num_channels == WAV_file::NUM_CHANNELS::ONE) {
		lastErrorMSG = "ERROR: getBuffer_8_bits_stereo() can't be used for a mono WAV file!";
		return -1;
	}
	//newBuffer_1_left = (uint8_t*)malloc(waveHeader.datachunk_size/2);
	newBuffer_1_left = new (nothrow) uint8_t[waveHeader.datachunk_size / 2];
	if (newBuffer_1_left == nullptr) {
		lastErrorMSG = "Error allocating memory";
		return -1;
	}
	newBuffer_2_right = new (nothrow) uint8_t[waveHeader.datachunk_size / 2];
	if (newBuffer_2_right == nullptr) {
		lastErrorMSG = "Error allocating memory";
		return -1;
	}
	if (bufferInternal_uint8_t != nullptr) {
		for (unsigned int i = 0; i < waveHeader.datachunk_size; i += 2) {
			newBuffer_1_left[i / 2] = bufferInternal_uint8_t[i];
			newBuffer_2_right[i / 2] = bufferInternal_uint8_t[i + 1];
		}
	} else {
		// Initializes the buffer the center value.
		memset(newBuffer_1_left,  _UI8_MAX / 2, waveHeader.datachunk_size / 2);
		memset(newBuffer_2_right, _UI8_MAX / 2, waveHeader.datachunk_size / 2);
	}
	return waveHeader.datachunk_size/2;
}

// NOTE: If no file have been reade it return an allocated and initialized buffer.
int WAV_file::getBuffer_16_bits_mono(int16_t* & newBuffer_1) {
	newBuffer_1 = nullptr;
	if (waveHeader.num_channels == WAV_file::NUM_CHANNELS::TWO) {
		lastErrorMSG = "ERROR: getBuffer_16_bits_mono() can't be used for a stereo WAV file!";
		return -1;
	}

	// newBuffer_1 = (int16_t*)malloc(waveHeader.datachunk_size);
	newBuffer_1 = new (nothrow) int16_t[waveHeader.datachunk_size/sizeof(int16_t)];
	if (newBuffer_1 == nullptr) {
		lastErrorMSG = "Error allocating memory";
		return -1;
	}
	if (bufferInternal_int16_t != nullptr) {
		copy(bufferInternal_int16_t,
			bufferInternal_int16_t + (waveHeader.datachunk_size / 2),
			newBuffer_1);
	} else {
		// Initializes the buffer the center value.
		memset(newBuffer_1, 0, waveHeader.datachunk_size);
	}
	return waveHeader.datachunk_size / 2; 
}

// NOTE: If no file have been reade it return an allocated and initialized buffer.
int WAV_file::getBuffer_16_bits_stereo(int16_t* & newBuffer_1_left, int16_t* & newBuffer_2_right) {
	newBuffer_1_left  = nullptr;
	newBuffer_2_right = nullptr;
	if (waveHeader.num_channels == WAV_file::NUM_CHANNELS::ONE) {
		lastErrorMSG = "ERROR: getBuffer_8_bits_stereo() can't be used for a mono WAV file!";
		return -1;
	}
	// newBuffer_1_left = (int16_t*)malloc(waveHeader.datachunk_size / 2);
	newBuffer_1_left = new (nothrow) int16_t[(waveHeader.datachunk_size / 2) / sizeof(int16_t) ];
	if (newBuffer_1_left == nullptr) {
		lastErrorMSG = "Error allocating memory";
		return -1;
	}
	newBuffer_2_right = new (nothrow) int16_t[(waveHeader.datachunk_size / 2) / sizeof(int16_t)];
	if (newBuffer_2_right == nullptr) {
		lastErrorMSG = "Error allocating memory";
		return -1;
	}
	if (bufferInternal_int16_t != nullptr) {
		for (unsigned int i = 0; i < (waveHeader.datachunk_size / 2); i += 2) {
			newBuffer_1_left[i / 2] = bufferInternal_int16_t[i];
			newBuffer_2_right[i / 2] = bufferInternal_int16_t[i + 1];
		}
	} else {
		// Initializes the buffer the center value.
		memset(newBuffer_1_left,  0, waveHeader.datachunk_size / 2);
		memset(newBuffer_2_right, 0, waveHeader.datachunk_size / 2);
	}
	return waveHeader.datachunk_size / 4;
}

/////////////////
// Set Buffers
////////////////

int WAV_file::setBuffer_8_bits_mono(uint8_t* & newBuffer_1) {
	if (newBuffer_1 == nullptr) {
		lastErrorMSG = "ERROR: setBuffer_8_bits_mono() newBuffer_1 is NULL!";
		return -1;
	}
	if (numberOfSamples == 0) {
		lastErrorMSG = "WARNING: setBuffer_8_bits_mono() the length of the buffer is zero!";
		return -1;
	}
	if (waveHeader.num_channels == WAV_file::NUM_CHANNELS::TWO) {
		lastErrorMSG = "ERROR: setBuffer_8_bits_mono() can't be used for a stereo WAV file!";
		return -1;
	}
	if (bufferInternal_uint8_t != nullptr) {
		delete[] bufferInternal_uint8_t;
		bufferInternal_uint8_t = nullptr;
	}
	bufferInternal_uint8_t = new /*(nothrow)*/ uint8_t[numberOfSamples];
	if (bufferInternal_uint8_t == nullptr) {
		lastErrorMSG = "Error allocating memory";
		return -1;
	}

	copy(newBuffer_1,
		 newBuffer_1 + numberOfSamples,
		 bufferInternal_uint8_t);

	return 0;
}

int WAV_file::setBuffer_8_bits_stereo(uint8_t* & newBuffer_1_left, uint8_t* & newBuffer_2_right) {
	if (newBuffer_1_left == nullptr) {
		lastErrorMSG = "ERROR: setBuffer_8_bits_stereo() newBuffer_1_left is NULL!";
		return -1;
	}
	if (newBuffer_2_right == nullptr) {
		lastErrorMSG = "ERROR: setBuffer_8_bits_stereo() newBuffer_2_right is NULL!";
		return -1;
	}
	if (numberOfSamples == 0) {
		lastErrorMSG = "ERROR: setBuffer_8_bits_stereo() the numberOfSamples of the buffer is zero!";
		return -1;
	}
	if (waveHeader.num_channels == WAV_file::NUM_CHANNELS::ONE) {
		lastErrorMSG = "ERROR: setBuffer_8_bits_stereo() can't be used for a stereo WAV file!";
		return -1;
	}

	if (bufferInternal_uint8_t != nullptr) {
		delete[] bufferInternal_uint8_t;
		bufferInternal_uint8_t = nullptr;
	}
	
	bufferInternal_uint8_t = new (nothrow) uint8_t[numberOfSamples*2];
	if (bufferInternal_uint8_t == nullptr) {
		lastErrorMSG = "Error allocating memory";
		return -1;
	}

	for (unsigned int i = 0; i < (numberOfSamples*2); i += 2) {
		bufferInternal_uint8_t[i]     = newBuffer_1_left[i / 2];
		bufferInternal_uint8_t[i + 1] = newBuffer_2_right[i / 2];
	}

	return 0;
}

int WAV_file::setBuffer_16_bits_mono(int16_t* & newBuffer_1) {
	if (newBuffer_1 == nullptr) {
		lastErrorMSG = "ERROR: setBuffer_16_bits_mono() newBuffer_1 is NULL!";
		return -1;
	}
	if (numberOfSamples == 0) {
		lastErrorMSG = "ERROR: setBuffer_16_bits_mono() the numberOfSamples of the buffer is zero!";
		return -1;
	}
	if (waveHeader.num_channels == WAV_file::NUM_CHANNELS::TWO) {
		lastErrorMSG = "ERROR: getBuffer_16_bits_mono() can't be used for a stereo WAV file!";
		return -1;
	}
	if (bufferInternal_int16_t != nullptr) {
		delete[] bufferInternal_int16_t;
		bufferInternal_int16_t = nullptr;
	}
	bufferInternal_int16_t = new (nothrow) int16_t[numberOfSamples];
	if (bufferInternal_int16_t == nullptr) {
		lastErrorMSG = "Error allocating memory";
		return -1;
	}

	copy(newBuffer_1,
		newBuffer_1 + numberOfSamples,  // This is pointer arithmetic.
		bufferInternal_int16_t);

	return 0;
}

int WAV_file::setBuffer_16_bits_stereo(int16_t* & newBuffer_1_left, int16_t* & newBuffer_2_right) {
	if (newBuffer_1_left == nullptr) {
		lastErrorMSG = "ERROR: setBuffer_16_bits_stereo() newBuffer_1_left is NULL!";
		return -1;
	}
	if (newBuffer_2_right == nullptr) {
		lastErrorMSG = "ERROR: setBuffer_16_bits_stereo() newBuffer_2_right is NULL!";
		return -1;
	}
	if (numberOfSamples == 0) {
		lastErrorMSG = "ERROR: setBuffer_16_bits_stereo() the numberOfSamples of the buffer is zero!";
		return -1;
	}
	if (waveHeader.num_channels == WAV_file::NUM_CHANNELS::ONE) {
		lastErrorMSG = "ERROR: setBuffer_16_bits_stereo() can't be used for a stereo WAV file!";
		return -1;
	}

	if (bufferInternal_int16_t != nullptr) {
		delete[] bufferInternal_int16_t;
		bufferInternal_int16_t = nullptr;
	}
	bufferInternal_int16_t = new (nothrow) int16_t[numberOfSamples * 2];
	if (bufferInternal_int16_t == nullptr) {
		lastErrorMSG = "Error allocating memory";
		return -1;
	}

	for (unsigned int i = 0; i < (numberOfSamples * 2); i += 2) {
		bufferInternal_int16_t[i]     = newBuffer_1_left[i / 2];
		bufferInternal_int16_t[i + 1] = newBuffer_2_right[i / 2];
	}

	return 0;
}

