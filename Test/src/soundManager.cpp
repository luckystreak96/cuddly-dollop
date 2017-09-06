#include "soundManager.h"

SoundManager::SoundManager()
{
	ALCcontext* context;

	// Initialize Open AL
	ALCdevice* device = alcOpenDevice(nullptr); // open default device
	if (device != 0) {
		context = alcCreateContext(device, nullptr); // create context
		if (context != nullptr) {
			alcMakeContextCurrent(context); // set active context
		}
	}

	CheckErrors();

	//ALboolean enumeration;

	//enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	//if (enumeration != AL_FALSE)
	//{
	//	const ALCchar *devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	//	const ALCchar *device = devices, *next = devices + 1;
	//	size_t len = 0;

	//	fprintf(stdout, "Devices list:\n");
	//	fprintf(stdout, "----------\n");
	//	while (device && *device != '\0' && next && *next != '\0') {
	//		fprintf(stdout, "%s\n", device);
	//		len = strlen(device);
	//		device += (len + 1);
	//		next += (len + 2);
	//	}
	//	fprintf(stdout, "----------\n");
	//}


	//Setup listener
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	alListener3f(AL_POSITION, 0, 0, 1.0f);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listenerOri);

	CheckErrors();


	//Generate sources
	ALuint source;
	alGenSources((ALuint)1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	CheckErrors();

	//Buffer generation
	ALuint buffer;

	alGenBuffers((ALuint)1, &buffer);


	//Loading buffer
	CWave file = CWave();
	file.Load("res/audio/fx/swish_2.wav");
	ALsizei size = file.GetSize();
	ALsizei freq = file.GetSampleRate();
	ALenum format = file.GetBitsPerSample();
	ALvoid *data = file.GetData();
	ALboolean loop = AL_FALSE;

	alutLoadWAVFile("res/audio/fx/swish_2.wav", &format, &data, &size, &freq, &loop);

	//set buffer data
	alBufferData(buffer, format, data, size, freq);


	CheckErrors();

	//Set the source
	alSourcei(source, AL_BUFFER, buffer);

	CheckErrors();

	alSourcePlay(source);

	ALint source_state;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	}

	CheckErrors();

	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);

	CheckErrors();
}

void SoundManager::CheckErrors()
{
	ALCenum error;
	error = alGetError();
	if (error != AL_NO_ERROR)
		std::cout << alGetString(error) << std::endl;
}