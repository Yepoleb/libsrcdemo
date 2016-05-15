#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>

#include "bitbuffer.hpp"
#include "netmessage.hpp"
#include "config.hpp"
#include "helpers.hpp"
#include "common.hpp"
#include "packet.hpp"

PacketMsg::PacketMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st) :
    DemoMessage(tick)
{
    readBuffer(data, data_size, parser_st);
}

void PacketMsg::readBuffer(const char* data, const size_t data_size, ParserState* parser_st)
{
    BitBuffer buf(data, data_size * 8);
    while (buf.bitsLeft() > 6) {
        NetMsgType msg_type = static_cast<NetMsgType>(buf.readBits(6));
        NetMsg* p_msg;
        switch (msg_type) {
            case NetMsgType::NET_NOP:
                p_msg = new NET_Nop(buf, parser_st);
                break;
            case NetMsgType::NET_DISCONNECT:
                p_msg = new NET_Disconnect(buf, parser_st);
                break;
            case NetMsgType::NET_FILE:
                p_msg = new NET_File(buf, parser_st);
                break;
            case NetMsgType::NET_TICK:
                p_msg = new NET_Tick(buf, parser_st);
                break;
            case NetMsgType::NET_STRINGCMD:
                p_msg = new NET_StringCmd(buf, parser_st);
                break;
            case NetMsgType::NET_SETCONVAR:
                p_msg = new NET_SetConVar(buf, parser_st);
                break;
            case NetMsgType::NET_SIGNONSTATE:
                p_msg = new NET_SignonState(buf, parser_st);
                break;
            case NetMsgType::SVC_PRINT:
                p_msg = new SVC_Print(buf, parser_st);
                break;
            case NetMsgType::SVC_SERVERINFO:
                p_msg = new SVC_ServerInfo(buf, parser_st);
                break;
            case NetMsgType::SVC_SENDTABLE:
                p_msg = new SVC_SendTable(buf, parser_st);
                break;
            case NetMsgType::SVC_CLASSINFO:
                p_msg = new SVC_ClassInfo(buf, parser_st);
                break;
            case NetMsgType::SVC_SETPAUSE:
                p_msg = new SVC_SetPause(buf, parser_st);
                break;
            case NetMsgType::SVC_CREATESTRINGTABLE:
                p_msg = new SVC_CreateStringTable(buf, parser_st);
                break;
            case NetMsgType::SVC_UPDATESTRINGTABLE:
                p_msg = new SVC_UpdateStringTable(buf, parser_st);
                break;
            case NetMsgType::SVC_VOICEINIT:
                p_msg = new SVC_VoiceInit(buf, parser_st);
                break;
            case NetMsgType::SVC_VOICEDATA:
                p_msg = new SVC_VoiceData(buf, parser_st);
                break;
            case NetMsgType::SVC_SOUNDS:
                p_msg = new SVC_Sounds(buf, parser_st);
                break;
            case NetMsgType::SVC_SETVIEW:
                p_msg = new SVC_SetView(buf, parser_st);
                break;
            case NetMsgType::SVC_FIXANGLE:
                p_msg = new SVC_FixAngle(buf, parser_st);
                break;
            case NetMsgType::SVC_CROSSHAIRANGLE:
                p_msg = new SVC_CrosshairAngle(buf, parser_st);
                break;
            case NetMsgType::SVC_BSPDECAL:
                p_msg = new SVC_BSPDecal(buf, parser_st);
                break;
            case NetMsgType::SVC_USERMESSAGE:
                p_msg = new SVC_UserMessage(buf, parser_st);
                break;
            case NetMsgType::SVC_ENTITYMESSAGE:
                p_msg = new SVC_EntityMessage(buf, parser_st);
                break;
            case NetMsgType::SVC_GAMEEVENT:
                p_msg = new SVC_GameEvent(buf, parser_st);
                break;
            case NetMsgType::SVC_PACKETENTITIES:
                p_msg = new SVC_PacketEntities(buf, parser_st);
                break;
            case NetMsgType::SVC_TEMPENTITIES:
                p_msg = new SVC_TempEntities(buf, parser_st);
                break;
            case NetMsgType::SVC_PREFETCH:
                p_msg = new SVC_Prefetch(buf, parser_st);
                break;
            case NetMsgType::SVC_MENU:
                p_msg = new SVC_Menu(buf, parser_st);
                break;
            case NetMsgType::SVC_GAMEEVENTLIST:
                p_msg = new SVC_GameEventList(buf, parser_st);
                break;
            case NetMsgType::SVC_GETCVARVALUE:
                p_msg = new SVC_GetCvarValue(buf, parser_st);
                break;
            default:
                std::cerr << "Unsupported message " << (int)msg_type << std::endl;
                p_msg = nullptr;
                break;
        }

        if (p_msg != nullptr) {
#ifdef DEBUG_PACKETS
            std::cout << indent(p_msg->toString(), 2);
#endif
            messages.push_back(p_msg);
        } else {
            break;
        }
    }
}

PacketMsg::~PacketMsg()
{
    for (NetMsg* msg : messages) {
        delete msg;
    }
}

std::string PacketMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << getName() << std::endl;
    for (NetMsg* msg : messages) {
        ss << indent(msg->toString(), 2);
    }
    return ss.str();
}
