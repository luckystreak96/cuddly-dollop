#ifndef AUDIO_COMPONENT_H__
#define AUDIO_COMPONENT_H__

#include "iComponent.h"
#include "vector3f.h"
#include "soundManager.h"
#include <string>
#include <AL/al.h>
#include <AL/alc.h>

class AudioComponent : public IComponent
{
public:
	virtual void ReceiveMessage(std::vector<std::string> message);

	AudioComponent();
	~AudioComponent();
	void PlaySound(std::string path = "res/audio/fx/swish_2.wav");
	void SetPitch(float pitch);
	void SetGain(float gain);
	void SetPosition(Vector3f pos);
	void SetVelocity(Vector3f vel);
	void SetLoop(bool loop);
	void LoadSound(std::string path);

private:
	void SetupSource();

private:
	std::string m_path;
	SourceInfo m_source;
};

#endif // !AUDIO_COMPONENT_H__

