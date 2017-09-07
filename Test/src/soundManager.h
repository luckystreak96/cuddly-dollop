#ifndef SOUND_MANAGER_H__
#define SOUND_MANAGER_H__

#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alut.h>
#include <iostream>
#include <map>
#include "vector3f.h"

struct WavBuffer
{
	ALuint buffer = 0;
	ALsizei size;
	ALsizei freq;
	ALenum format;
	ALvoid *data;
	ALboolean loop = AL_TRUE;
};

struct SourceInfo
{
	ALuint source = 0;
	ALfloat pitch = 1;
	ALfloat gain = 1;
	Vector3f pos;
	Vector3f vel;
	ALboolean loop = AL_TRUE;
};

class SoundManager
{
public:
	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	};

	void SetListenerPosition(Vector3f pos = Vector3f(), Vector3f vel = Vector3f());
	void SetListenerOrientation(unsigned int dir);
	unsigned int CreateSource();
	void DeleteSource(unsigned int source);
	void SetPitch(unsigned int source, float pitch);
	void SetGain(unsigned int source, float gain);
	void SetPosition(unsigned int source, Vector3f pos);
	void SetVelocity(unsigned int source, Vector3f vel);
	void SetLoop(unsigned int source, bool loop);
	void CreateBuffer(std::string path = "res/audio/fx/swish_2.wav");
	void Play(unsigned int source, std::string path);
private:
	SoundManager();
	~SoundManager();
	void SetSource(unsigned int source, unsigned int buffer);
	void CheckErrors();

private:
	std::map<std::string, WavBuffer> m_buffers;
	ALCcontext* m_context;
	ALCdevice* m_device;
};

#endif // !SOUND_MANAGER_H__
