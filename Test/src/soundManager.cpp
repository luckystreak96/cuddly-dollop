#include "soundManager.h"
#include "elapsedTime.h"


SoundManager::SoundManager() : m_bgmSource(0), m_bgmState(BGM_Starting), m_bgmVolume(0), m_bgmMaxVolume(0.4f), m_masterVolume(1.0f)
{
	// Initialize Open AL
	m_device = alcOpenDevice(nullptr); // open default device
	if (m_device != 0) {
		char* DefaultDevice = (char*)alcGetString(m_device, ALC_DEFAULT_DEVICE_SPECIFIER);
		std::cout << DefaultDevice << std::endl;
		m_context = alcCreateContext(m_device, nullptr); // create context
		if (m_context != nullptr) {
			alcMakeContextCurrent(m_context); // set active context
		}
	}

	CheckErrors();

	//Setup listener
	SetListenerPosition();
	alListenerf(AL_GAIN, 1.0f);
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	alListenerfv(AL_ORIENTATION, listenerOri);

	m_bgmSource = CreateSource();
	//CreateBuffer();

	////Generate sources
	//ALuint source;
	//alGenSources((ALuint)1, &source);
	//alSourcef(source, AL_PITCH, 1);
	//alSourcef(source, AL_GAIN, 1);
	//alSource3f(source, AL_POSITION, 0, 0, 0);
	//alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(m_bgmSource, AL_LOOPING, AL_TRUE);

	CheckErrors();
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
	// Clear buffers
	for (auto &x : m_buffers)
		alDeleteSources(1, &x.second.buffer);

	// Clear BGM source
	alDeleteSources(1, &m_bgmSource);

	// Free AL context
	m_device = alcGetContextsDevice(m_context);
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);

	CheckErrors();
}

void SoundManager::PlaySoundFX(std::string sourceFile)
{
	if (sourceFile == "")
		return;
	CreateBuffer(sourceFile);
	ALuint source = CreateSource();
	Play(source, sourceFile);
	m_sfxSources.push_back(source);
}

void SoundManager::SetBGM(std::string sourceFile)
{
	if (sourceFile == m_currentBGM)
		return;
	if (sourceFile != "")
	{
		m_nextBGM = sourceFile;
		CreateBuffer(sourceFile);
	}

	m_bgmState = BGM_Stopping;
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

void SoundManager::Stop(unsigned int source)
{
	if (source == 0)
	{
		std::cout << "Invalid source for Stop()." << std::endl;
		return;
	}

	if (alIsSource(source))
	{
		ALint source_state;
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		if (source_state == AL_PLAYING) {
			alSourceStop(source);
		}
	}

	CheckErrors();
}

bool SoundManager::IsPlaying(unsigned int source)
{
	ALint source_state;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	if (source_state == AL_PLAYING) {
		return true;
	}

	CheckErrors();

	return false;
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
	alBufferData(buf.buffer, buf.format, buf.data, buf.size, buf.freq);
	//alBufferData(buf.buffer, AL_FORMAT_STEREO16, buf.data, buf.size, buf.freq);

	CheckErrors();

	m_buffers.emplace(path, buf);
}

void SoundManager::SetListenerPosition(Vector3f pos, Vector3f vel)
{
	alListener3f(AL_POSITION, (ALfloat)pos.x, (ALfloat)pos.y, (ALfloat)pos.z);
	//alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
	//CheckErrors();
	CheckErrors();
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
	if (alIsSource(source))
		alDeleteSources((ALsizei)1, &source);
	CheckErrors();
}

void SoundManager::Update()
{
	for (int i = 0; i < m_sfxSources.size(); i++)
	{
		ALuint source = m_sfxSources[i];
		if (!IsPlaying(source))
		{
			DeleteSource(source);
			m_sfxSources.erase(m_sfxSources.begin() + i);
			i--;
		}
	}

	if (m_bgmState == BGM_Starting)
	{
		if (m_bgmVolume < m_bgmMaxVolume)
		{
			m_bgmVolume += 0.6f * (float)ElapsedTime::GetInstance().GetElapsedTime();
			m_bgmVolume = fmin(m_bgmMaxVolume, m_bgmVolume);
		}
		else
		{
			m_bgmState = BGM_Playing;
			m_bgmVolume = m_bgmMaxVolume;
		}
	}
	else if (m_bgmState == BGM_Stopping)
	{
		if (m_bgmVolume > 0.0f)
		{
			m_bgmVolume -= 0.6f * (float)ElapsedTime::GetInstance().GetElapsedTime();
			m_bgmVolume = fmax(0.0f, m_bgmVolume);
		}
		else
		{
			m_bgmVolume = 0;

			ALint source_state;
			alGetSourcei(m_bgmSource, AL_SOURCE_STATE, &source_state);
			if (source_state == AL_PLAYING) {
				alSourceStop(m_bgmSource);
			}
			CheckErrors();

			if (m_nextBGM == "")
				m_bgmState = BGM_Playing;
			else
			{
				m_bgmState = BGM_Starting;
				SetSource(m_bgmSource, m_buffers.at(m_nextBGM).buffer);
				CheckErrors();

				alSourcePlay(m_bgmSource);
				CheckErrors();
			}
			m_currentBGM = m_nextBGM;
			m_nextBGM = "";
		}
	}

	alSourcef(m_bgmSource, AL_GAIN, m_bgmVolume);
	CheckErrors();
}

unsigned int SoundManager::CreateSource()
{
	ALuint source;
	alGenSources((ALuint)1, &source);

	CheckErrors();

	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1.f);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

	//float value = 0;
	//alGetSourcef(source, AL_REFERENCE_DISTANCE, &value);
	//std::cout << "Default Reference Distance: " << value << std::endl;
	alSourcef(source, AL_REFERENCE_DISTANCE, 10);
	alSourcef(source, AL_MAX_DISTANCE, 100);
	//alGetSourcef(source, AL_MAX_DISTANCE, &value);
	//std::cout << "Max Distance: " << value << std::endl;
	CheckErrors();

	return source;
}

void SoundManager::SetPitch(unsigned int source, float pitch)
{
	alSourcef(source, AL_PITCH, pitch);
	CheckErrors();
}

void SoundManager::SetGain(unsigned int source, float gain)
{
	alSourcef(source, AL_GAIN, gain);
	CheckErrors();
}

void SoundManager::SetPosition(unsigned int source, Vector3f pos)
{
	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
	CheckErrors();
}

void SoundManager::SetVelocity(unsigned int source, Vector3f vel)
{
	alSource3f(source, AL_VELOCITY, vel.x, vel.y, vel.z);
	CheckErrors();
}

void SoundManager::SetLoop(unsigned int source, bool loop)
{
	alSourcei(source, AL_LOOPING, loop);
	CheckErrors();
}


void SoundManager::CheckErrors()
{
	ALCenum error;
	error = alGetError();
	if (error != AL_NO_ERROR)
		std::cout << alGetString(error) << std::endl;
}

void SoundManager::SetMasterVolume(float volume)
{
	m_masterVolume = volume;
	alListenerf(AL_GAIN, m_masterVolume);
	CheckErrors();
}