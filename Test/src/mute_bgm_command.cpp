//
// Created by yanik on 08/03/19.
//

#include "mute_bgm_command.h"

#include "soundManager.h"


MuteBGMCommand::MuteBGMCommand()
{
}

void MuteBGMCommand::execute()
{
    SoundManager::GetInstance().mute_BGM();
}