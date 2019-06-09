#ifndef PLAYER_AUDIO_COMPONENT_H__
#define PLAYER_AUDIO_COMPONENT_H__

#include "audioComponent.h"
#include "observer.h"

class PlayerAudioComponent : public AudioComponent
{
public:
	PlayerAudioComponent() {}
	void Update(Observable* obs);

private:
	int m_prevFootstep;
};

#endif