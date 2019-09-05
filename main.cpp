/******************************************************************************
 *                                                                            *
 *                             WAV Tools C++                                  *
 *                                                                            *
 * ****************************************************************************
 * Author: Joao Nuno Carvalho                                                 *
 * Date:   2019.09.05                                                         *
 * License: MIT Open Source License.                                          *
 * Description: This is the beginnings of a lib in C++ to manipulate and      *
 *              process WAV files.                                            *
 * ****************************************************************************
*/


#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "main.h"
#include "WAV_file.h"
#include "WAV_processor.h"

using namespace std;

int main()
{
	cout << "WAVE_TOOLS" << endl << endl;

	cout << "testReadWriteWAVFile()" << endl;
	testReadWriteWAVFile();
	cout << "End testReadWriteWAVFile()" << endl;

	cout << endl << "generate8Bits440HzWAVFile()" << endl;
	generate8Bits440HzWAVFile();
	cout << "End generate8Bits440HzWAVFile()" << endl;

	cout << endl << "generate16Bits440HzWAVFile()" << endl;
	generate16Bits440HzWAVFile();
	cout << "End generate16Bits440HzWAVFile()" << endl;

	cout << endl << "WAV_processor func" << endl;
	WAV_processor_func();
	cout << "End WAV_processor func" << endl;

	cin.get();

	return 0;
}

/////////////
// Functions

int testReadWriteWAVFile() {
	WAV_file my_wav_file = WAV_file();
	my_wav_file.setName("dog_bark2.wav");
	cout << "Nome: " << my_wav_file.getName() << endl;
	my_wav_file.setPath(".\\wav_files\\");
	cout << "Path: " << my_wav_file.getPath() << endl;

	bool error_ocurred = my_wav_file.loadFile();
	if (error_ocurred) {
		cout << my_wav_file.getErrorMSG() << endl;
		return -1;
	}
	my_wav_file.printWAVHeader();

	uint8_t * buffer_8_bits_Mono;
	uint32_t numberOfSamples = my_wav_file.getBuffer_8_bits_mono(buffer_8_bits_Mono);

	// Buffer processing.
	cout << endl << endl;
	for (unsigned int i = 0; i < numberOfSamples; i++) {
//		cout << (unsigned short int) buffer_8_bits_Mono[i] << '.';
	}
	cout << endl << endl;

	// We are going to save the file on the hard drive with another name.
	my_wav_file.setName("dog_bark2_saved.wav");
	my_wav_file.initializeWaveHeaderStructBeforeWriting();
	my_wav_file.setBuffer_8_bits_mono(buffer_8_bits_Mono);
	delete[] buffer_8_bits_Mono;
	my_wav_file.writeFile();
	return 0;
}

int generate8Bits440HzWAVFile() {
	WAV_file my_wav_file = WAV_file();
	my_wav_file.setName("A440_8_Bits.wav");
	cout << "Nome: " << my_wav_file.getName() << endl;
	my_wav_file.setPath(".\\wav_files\\");
	cout << "Path: " << my_wav_file.getPath() << endl;

	const uint32_t C_SAMPLES_PER_SEC = 8000;
	const uint32_t C_TIME_SEC = 5;

	my_wav_file.setBitsPerSample(WAV_file::BPS_8_BITS);
	my_wav_file.setNumChannels(WAV_file::NUM_CHANNELS::ONE);
	my_wav_file.setSampleRate(C_SAMPLES_PER_SEC);
	my_wav_file.setNumberOfSamples(C_SAMPLES_PER_SEC * C_TIME_SEC); // 5 seconds

	my_wav_file.initializeWaveHeaderStructBeforeWriting();

	uint8_t * buffer_8_bits_Mono;
	// Allocate the memory for the buffer.
	my_wav_file.getBuffer_8_bits_mono(buffer_8_bits_Mono);


	// Fills the buffer with a sinusoidal.
	float alphaAngle = 0;
	const uint32_t C_SIGNAL_FREQUENCY = 440; // 440 Hz La (A)
	for (uint32_t i = 0; i < my_wav_file.getNumberOfSamples(); i++) {
		buffer_8_bits_Mono[i] = uint8_t(roundf((sin(alphaAngle) /*/ 2*/) * (_UI8_MAX / 2) + (_UI8_MAX / 2)));
		alphaAngle += (2 * float(C_PI) * C_SIGNAL_FREQUENCY) / C_SAMPLES_PER_SEC;
	}

	my_wav_file.setBuffer_8_bits_mono(buffer_8_bits_Mono);	
	delete[] buffer_8_bits_Mono;
	my_wav_file.writeFile();
	return 0;
}


int generate16Bits440HzWAVFile() {
	WAV_file my_wav_file = WAV_file();
	my_wav_file.setName("A440_16_Bits.wav");
	cout << "Nome: " << my_wav_file.getName() << endl;
	my_wav_file.setPath(".\\wav_files\\");
	cout << "Path: " << my_wav_file.getPath() << endl;

	const uint32_t C_SAMPLES_PER_SEC = 8000;
	const uint32_t C_TIME_SEC = 5;

	my_wav_file.setBitsPerSample(WAV_file::BPS_16_BITS);
	my_wav_file.setNumChannels(WAV_file::NUM_CHANNELS::ONE);
	my_wav_file.setSampleRate(C_SAMPLES_PER_SEC);
	my_wav_file.setNumberOfSamples(C_SAMPLES_PER_SEC * C_TIME_SEC); // 5 seconds

	my_wav_file.initializeWaveHeaderStructBeforeWriting();

	int16_t * buffer_16_bits_Mono;
	// Allocate the memory for the buffer.
	my_wav_file.getBuffer_16_bits_mono(buffer_16_bits_Mono);


	// Fills the buffer with a sinusoidal.
	float alphaAngle = 0;
	const uint32_t C_SIGNAL_FREQUENCY = 440; // 440 Hz La (A)
	for (uint32_t i = 0; i < my_wav_file.getNumberOfSamples(); i++) {
		buffer_16_bits_Mono[i] = int16_t(roundf( sin(alphaAngle) * _I16_MAX  ));
		alphaAngle += (2 * float(C_PI) * C_SIGNAL_FREQUENCY) / C_SAMPLES_PER_SEC;
	}

	my_wav_file.setBuffer_16_bits_mono(buffer_16_bits_Mono);
	delete[] buffer_16_bits_Mono;
	my_wav_file.writeFile();
	return 0;
}

int WAV_processor_func() {
	string inputFileName = "dog_bark2_16Bits.wav";
	string inputPath = ".\\wav_files\\";
	WAV_processor my_wav_proc = WAV_processor(inputFileName, inputPath);
	my_wav_proc.loadInputFile();
	my_wav_proc.process();

	return 0;
}

