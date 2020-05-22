#pragma once

#include <iostream>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <sys/time.h>


// ------------------------------ Class microphone --------------------------------
class Microphone {
private:
	const ALCchar *   device;
   	ALCdevice *       captureDev;

public:
	//Microphone();
	//~Microphone();

	void OpenDevice();
	void CaptureStart();
	void CaptureStop();
	void GetAvailableSamples(unsigned char *captureBufPtr, int32_t &samplesCaptured, int32_t &bytes);
	void CloseDevice();
};


// ------------------------------ Class audio-player --------------------------------
class AudioPlayer {
private:
	const ALCchar *   devices;
   	ALCdevice *       mainDev;
   	ALCcontext *      mainContext;

   	ALuint            buffer;
   	ALuint            source;
   	ALint             playState;

public:
	//AudioPlayer();
	//~AudioPlayer();

	void OpenDevice();
	void CreateContext();
	void Play(unsigned char *captureBuffer, int samplesCaptured);
	void CloseDevice();
};