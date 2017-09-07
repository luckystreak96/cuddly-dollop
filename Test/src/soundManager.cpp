#include "soundManager.h"

SoundManager::SoundManager()
{
	// Initialize Open AL
	m_device = alcOpenDevice(nullptr); // open default device
	if (m_device != 0) {
		m_context = alcCreateContext(m_device, nullptr); // create context
		if (m_context != nullptr) {
			alcMakeContextCurrent(m_context); // set active context
		}
	}

	CheckErrors();

	//Setup listener
	SetListenerPosition();
	alListenerf(AL_GAIN, 0.9f);
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f };
	alListenerfv(AL_ORIENTATION, listenerOri);


	CreateBuffer();

	////Generate sources
	//ALuint source;
	//alGenSources((ALuint)1, &source);
	//alSourcef(source, AL_PITCH, 1);
	//alSourcef(source, AL_GAIN, 1);
	//alSource3f(source, AL_POSITION, 0, 0, 0);
	//alSource3f(source, AL_VELOCITY, 0, 0, 0);
	//alSourcei(source, AL_LOOPING, AL_FALSE);

	//CheckErrors();

	//Buffer generation
	//ALuint buffer;

	//alGenBuffers((ALuint)1, &buffer);


	////Loading buffer
	//ALsizei size;
	//ALsizei freq;
	//ALenum format;
	//ALvoid *data;
	//ALboolean loop = AL_TRUE;

	//alutLoadWAVFile("res/audio/fx/swish_2.wav", &format, &data, &size, &freq, &loop);

	////set buffer data
	//alBufferData(buffer, format, data, size, freq);


	//CheckErrors();

	//Set the source
	//alSourcei(source, AL_BUFFER, buffer);

	//CheckErrors();

	//alSourcePlay(source);

	//ALint source_state;
	//alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	//while (source_state == AL_PLAYING) {
	//	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	//}

	//CheckErrors();

	//alDeleteSources(1, &source);
	//alDeleteBuffers(1, &buffer);
	//device = alcGetContextsDevice(context);
	//alcMakeContextCurrent(nullptr);
	//alcDestroyContext(context);
	//alcCloseDevice(device);

	//CheckErrors();
}

SoundManager::~SoundManager()
{
	m_device = alcGetContextsDevice(m_context);
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);

	CheckErrors();
}

void SoundManager::SetSource(unsigned int source, unsigned int buffer)
{
	alSourcei(source, AL_BUFFER, buffer);
	CheckErrors();
}

void SoundManager::Play(unsigned int source, std::string path)
{
	if (source == 0 || m_buffers.count(path) == 0)
	{
		std::cout << "Sound not loaded or invalid source." << std::endl;
		return;
	}

	SetSource(source, m_buffers.at(path).buffer);
	alSourcePlay(source);

	CheckErrors();
}

void SoundManager::CreateBuffer(std::string path)
{
	if (m_buffers.count(path))
		return;

	//Buffer generation
	WavBuffer buf;
	alGenBuffers((ALuint)1, &buf.buffer);

	alutLoadWAVFile((ALbyte*)path.c_str(), &buf.format, &buf.data, &buf.size, &buf.freq, &buf.loop);

	//set buffer data
	alBufferData(buf.buffer, AL_FORMAT_MONO16, buf.data, buf.size, buf.freq);

	CheckErrors();

	m_buffers.emplace(path, buf);
}

void SoundManager::SetListenerPosition(Vector3f pos, Vector3f vel)
{
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
	//alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
	//CheckErrors();
}

void SoundManager::SetListenerOrientation(unsigned int dir)
{
	Vector3f direction;
	if (dir == 0)
		direction = Vector3f(0, 1, 0);
	else if (dir == 1)
		direction = Vector3f(1, 0, 0);
	else if (dir == 2)
		direction = Vector3f(0, -1, 0);
	else if (dir == 3)
		direction = Vector3f(-1, 0, 0);

	ALfloat listenerOri[] = { direction.x, direction.y, direction.z, 0.0f, 0.0f, 1.0f };
	alListenerfv(AL_ORIENTATION, listenerOri);

	CheckErrors();
}

void SoundManager::DeleteSource(unsigned int source)
{
	alDeleteSources((ALsizei)1, &source);
}

unsigned int SoundManager::CreateSource()
{
	ALuint source;
	alGenSources((ALuint)1, &source);
	
	CheckErrors();

	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	//alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

	//float value = 0;
	////alGetSourcef(source, AL_REFERENCE_DISTANCE, &value);
	////std::cout << "Default Reference Distance: " << value << std::endl;
	////alSourcef(source, AL_REFERENCE_DISTANCE, 10);
	////alSourcef(source, AL_MAX_DISTANCE, 100);
	//alGetSourcef(source, AL_MAX_DISTANCE, &value);
	//std::cout << "Max Distance: " << value << std::endl;

	return source;
}

void SoundManager::SetPitch(unsigned int source, float pitch)
{
	alSourcef(source, AL_PITCH, pitch);
}

void SoundManager::SetGain(unsigned int source, float gain)
{
	alSourcef(source, AL_GAIN, gain);
}

void SoundManager::SetPosition(unsigned int source, Vector3f pos)
{
	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
}

void SoundManager::SetVelocity(unsigned int source, Vector3f vel)
{
	alSource3f(source, AL_VELOCITY, vel.x, vel.y, vel.z);
}

void SoundManager::SetLoop(unsigned int source, bool loop)
{
	alSourcei(source, AL_LOOPING, loop);
}


void SoundManager::CheckErrors()
{
	ALCenum error;
	error = alGetError();
	if (error != AL_NO_ERROR)
		std::cout << alGetString(error) << std::endl;
}