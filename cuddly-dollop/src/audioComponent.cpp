#include "audioComponent.h"

void AudioComponent::ReceiveMessage(std::vector<std::string> message)
{
	if (message.at(0) == "SET_POSITION")
	{
		Vector3f pos = Vector3f(::atof(message.at(1).c_str()), ::atof(message.at(2).c_str()), ::atof(message.at(3).c_str()));
		SetPosition(pos);
	}
}


AudioComponent::AudioComponent()
{
	//SetupSource();
	//SetLoop(true);
	//PlaySound();
}

AudioComponent::~AudioComponent()
{
	//Free source
	//if (m_source.source != 0)
	//	SoundManager::GetInstance().DeleteSource(m_source.source);
}

void AudioComponent::PlaySound(std::string path)
{
	SoundManager::GetInstance().Play(m_source.source, path);
}

void AudioComponent::SetupSource()
{
	//If theres already a source, free it
	if (m_source.source != 0)
		SoundManager::GetInstance().DeleteSource(m_source.source);
	else
		m_source.source = SoundManager::GetInstance().CreateSource();
}

void AudioComponent::SetPitch(float pitch)
{
	if (m_source.source != 0)
		SoundManager::GetInstance().SetPitch(m_source.source, pitch);

	m_source.pitch = pitch;
}

void AudioComponent::SetGain(float gain)
{
	if (m_source.source != 0)
		SoundManager::GetInstance().SetGain(m_source.source, gain);

	m_source.gain = gain;
}

void AudioComponent::SetPosition(Vector3f pos)
{
	if (m_source.source != 0)
		SoundManager::GetInstance().SetPosition(m_source.source, pos);

	m_source.pos = pos;
}

void AudioComponent::SetVelocity(Vector3f vel)
{
	if (m_source.source != 0)
		SoundManager::GetInstance().SetVelocity(m_source.source, vel);

	m_source.vel = vel;
}

void AudioComponent::SetLoop(bool loop)
{
	if (m_source.source != 0)
		SoundManager::GetInstance().SetLoop(m_source.source, loop);

	m_source.loop = loop;
}

void AudioComponent::LoadSound(std::string path)
{
	m_path = path;

	//If you dont specify anything just sweep
	if (m_path == "")
		SoundManager::GetInstance().CreateBuffer();
	else
		SoundManager::GetInstance().CreateBuffer(path);
}