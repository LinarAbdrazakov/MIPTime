#include "audio.hpp"



// ------------------------------ Class microphone --------------------------------
void Microphone::OpenDevice() {
	// Open the default device
	std::cout << "Opening capture device:" << std::endl;
	captureDev = alcCaptureOpenDevice(NULL, 8000, AL_FORMAT_MONO16, 800);
	if (captureDev == NULL)
	{
		std::cout <<"   Unable to open device!" << std::endl;
		exit(1);
	}
	device = alcGetString(captureDev, ALC_CAPTURE_DEVICE_SPECIFIER);
	std::cout << "   opened device " << device << std::endl;
}

void Microphone::CaptureStart() {
	std::cout << "Starting capture" << std::endl;
	alcCaptureStart(captureDev);
}

void Microphone::CaptureStop() {
	std::cout << "Pausing capture" << std::endl;
	alcCaptureStop(captureDev);
}

void Microphone::GetAvailableSamples(unsigned char *captureBufPtr, int32_t &samplesCaptured, int32_t &bytes) {
	ALint samplesAvailable;
	// Get the number of samples available
  	alcGetIntegerv(captureDev, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable);

    // Copy the samples to our capture buffer
    if (samplesAvailable > 0) {
        alcCaptureSamples(captureDev, captureBufPtr, samplesAvailable);
        samplesCaptured = samplesAvailable;
        std::cout << "Captured " << samplesCaptured << " samples (adding " << samplesAvailable << ")" << std::endl;

        // (two bytes per sample * number of samples)
        bytes = samplesAvailable * 2;
    }
}

void Microphone::CloseDevice() {
	alcCaptureCloseDevice(captureDev);
}



// ------------------------------ Class audio-player --------------------------------
void AudioPlayer::OpenDevice() {
	// Open a playback device and create a context first
	std::cout << "Opening playback device:" << std::endl;
	mainDev = alcOpenDevice(NULL);
   	if (mainDev == NULL) {
  		std::cout << "Unable to open playback device!" << std::endl;
        exit(1);
	}
	devices = alcGetString(mainDev, ALC_DEVICE_SPECIFIER);
	std::cout << "   opened device " << devices << std::endl;
}

void AudioPlayer::CreateContext() {
	mainContext = alcCreateContext(mainDev, NULL);
	if (mainContext == NULL) {
 		std::cout << "Unable to create playback context!" << std::endl;
  		exit(1);
	}
	std::cout << "   created playback context" << std::endl;

	// Make the playback context current
	alcMakeContextCurrent(mainContext);
	alcProcessContext(mainContext);
}

void AudioPlayer::Play(unsigned char *captureBuffer, int samplesCaptured) {
	static int not_first = 0;

	int bytes = 2 * samplesCaptured;
	unsigned char cloneBuffer[100000];
	for (int i = 0; i < bytes; ++i) {
		cloneBuffer[i] = captureBuffer[i];
	}

	// Wait for the source to stop playing
	if (not_first) {
		playState = AL_PLAYING;
		while (playState == AL_PLAYING) {
			alGetSourcei(source, AL_SOURCE_STATE, &playState);
		}
	}

	not_first = 1;

	alGenBuffers(1, &buffer);
	alGenSources(1, &source);
	alBufferData(buffer, AL_FORMAT_MONO16, cloneBuffer, samplesCaptured*2, 8000);

	alSourcei(source, AL_BUFFER, buffer);
	alSourcef(source, AL_GAIN, 1.0f);
	alListenerf(AL_GAIN, 1.0f);

	alSourcePlay(source);

	std::cout << "Played " << samplesCaptured << " samples." << std::endl;
}



void AudioPlayer::CloseDevice() {
	// Shut down OpenAL
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcMakeContextCurrent(NULL);
	alcCloseDevice(mainDev);
}


/*
int main() {
	Microphone mic;
	AudioPlayer player;

	mic.OpenDevice();
	player.OpenDevice();
	player.CreateContext();

	mic.CaptureStart();

	time_t startTime = time(NULL);
   	time_t currentTime = time(NULL);

   	unsigned char captureBuf[100000];
	int32_t samplesCaptured;
	int32_t bytes;
	int32_t bytes_per_second;

	while (currentTime - startTime < 20) {
		mic.GetAvailableSamples(captureBuf, samplesCaptured, bytes);
		player.Play(captureBuf, samplesCaptured);
		currentTime = time(NULL);

		bytes_per_second += bytes;
	}

	bytes_per_second /= 20;
	std::cout << "bytes_per_second: " << bytes_per_second << std::endl;
	mic.CaptureStop();

	mic.CloseDevice();
	player.CloseDevice();
} */