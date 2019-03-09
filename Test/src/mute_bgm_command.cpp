//
// Created by yanik on 08/03/19.
//

#include "mute_bgm_command.h"

#include "soundManager.h"


MuteBGMCommand::MuteBGMCommand()
{
    m_volume = SoundManager::GetInstance().GetBGMVolume();
}

void MuteBGMCommand::execute()
{
    float vol = SoundManager::GetInstance().GetBGMVolume();

    if(vol == 0.0f)
        SoundManager::GetInstance().SetBGMVolume(m_volume);
    else
        SoundManager::GetInstance().SetBGMVolume(0.0f);
}