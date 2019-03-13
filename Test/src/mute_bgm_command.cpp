//
// Created by yanik on 08/03/19.
//

#include "mute_bgm_command.h"

#include "soundManager.h"


MuteBGMCommand::MuteBGMCommand(std::string *str) : TextDisplayingCommand(str)
{
    *m_text = get_current_text_state();
}

void MuteBGMCommand::execute()
{
    SoundManager::GetInstance().toggle_BGM_mute();
    *m_text = get_current_text_state();
}

std::string MuteBGMCommand::get_current_text_state() {
    std::string result;
    bool muted = SoundManager::GetInstance().get_BGM_muted();
    if(muted)
        result = "Unmute BGM";
    else
        result = "Mute BGM";

    return result;
}
