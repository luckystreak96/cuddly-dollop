//
// Created by yanik on 12/03/19.
//

#include "text_displaying_command.h"

TextDisplayingCommand::TextDisplayingCommand(std::string* str)
{
    m_text = str;
}

std::string TextDisplayingCommand::get_current_text_state() {
    return "DEFAULT_VALUE FOR TEXTDISPLAYCOMMAND";
}
