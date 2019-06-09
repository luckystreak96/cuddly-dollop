#include "player_audio_component.h"

#include "playerGraphicsComponent.h"

void PlayerAudioComponent::Update(Observable* obs)
{
	PlayerGraphicsComponent* pgc = dynamic_cast<PlayerGraphicsComponent*>(obs);
	if (pgc != nullptr)
	{
		if (pgc->_animation >= AE_LeftMove)
		{
			int sprite = pgc->_sprite;
			if (sprite != m_prevFootstep)
			{
				m_prevFootstep = sprite;
				if (sprite == 2 || sprite == 4)
				{
					int num = (rand() % 3) + 1;
					SoundManager::GetInstance().PlaySoundFX("res/audio/fx/soft_footsteps_00" + std::to_string(num) + ".wav");
					//SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
				}
			}
		}
	}
}
