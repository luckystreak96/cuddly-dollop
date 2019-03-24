//
// Created by yanik on 23/03/19.
//

#include "toggle_bloom_command.h"
#include "renderer.h"

ToggleBloomCommand::ToggleBloomCommand(std::string *str) : TextDisplayingCommand(str)
{
    *m_text = get_current_text_state();
}

void ToggleBloomCommand::execute()
{
    Renderer::GetInstance().toggle_bloom();
    *m_text = get_current_text_state();
}

std::string ToggleBloomCommand::get_current_text_state() {
    std::string result;
    bool blooming = Renderer::GetInstance().get_bloom_state();
    if(blooming)
        result = "Bloom Enabled";
    else
        result = "Bloom Disabled";

    return result;
}
