/**
 * @file
 *
 * @Author OmniBlade
 *
 * @brief Command message list handling.
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "commandlist.h"
#include "gamemessage.h"

#ifndef THYME_STANDALONE
CommandList *&g_theCommandList = Make_Global<CommandList *>(0x00A29B78);
#else
CommandList *g_theCommandList = nullptr;
#endif

void CommandList::Destroy_All_Messages()
{
    GameMessage *next = nullptr;

    for (GameMessage *msg = m_firstMessage; msg != nullptr; msg = next) {
        next = msg->Get_Next();
        Delete_Instance(msg);
    }

    m_firstMessage = nullptr;
    m_lastMessage = nullptr;
}

void CommandList::Append_Message_List(GameMessage *list)
{
    for (GameMessage *msg = list; msg != nullptr;) {
        GameMessage *next = msg->Get_Next();
        Append_Message(msg);
        msg = next;
    }
}

CommandList::~CommandList()
{
    Destroy_All_Messages();
}

void CommandList::Reset()
{
    Destroy_All_Messages();
}
