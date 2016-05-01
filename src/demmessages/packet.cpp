#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>

#include "bitbuffer.hpp"
#include "netmessage.hpp"
#include "common.hpp"
#include "helpers.hpp"
#include "packet.hpp"

PacketMsg::PacketMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(tick)
{
    BitBuffer buf(data, data_size * 8);
    while (buf.BitsLeft() > 6) {
        NetMsgType msg_type = static_cast<NetMsgType>(buf.ReadBits(6));
        NetMsg* p_msg;
        switch (msg_type) {
            case NetMsgType::NET_NOP:
                p_msg = new NET_Nop(buf);
                break;
            case NetMsgType::NET_DISCONNECT:
                p_msg = new NET_Disconnect(buf);
                break;
            case NetMsgType::NET_FILE:
                p_msg = new NET_File(buf);
                break;
            case NetMsgType::NET_TICK:
                p_msg = new NET_Tick(buf);
                break;
            case NetMsgType::NET_STRINGCMD:
                p_msg = new NET_StringCmd(buf);
                break;
            case NetMsgType::NET_SETCONVAR:
                p_msg = new NET_SetConVar(buf);
                break;
            case NetMsgType::NET_SIGNONSTATE:
                p_msg = new NET_SignonState(buf);
                break;
            case NetMsgType::SVC_PRINT:
                p_msg = new SVC_Print(buf);
                break;
            case NetMsgType::SVC_SERVERINFO:
                p_msg = new SVC_ServerInfo(buf);
                break;
            case NetMsgType::SVC_SENDTABLE:
                p_msg = new SVC_SendTable(buf);
                break;
            case NetMsgType::SVC_CLASSINFO:
                p_msg = new SVC_ClassInfo(buf);
                break;
            case NetMsgType::SVC_SETPAUSE:
                p_msg = new SVC_SetPause(buf);
                break;
            case NetMsgType::SVC_CREATESTRINGTABLE:
                p_msg = new SVC_CreateStringTable(buf);
                break;
            case NetMsgType::SVC_UPDATESTRINGTABLE:
                p_msg = new SVC_UpdateStringTable(buf);
                break;
            case NetMsgType::SVC_VOICEINIT:
                p_msg = new SVC_VoiceInit(buf);
                break;
            case NetMsgType::SVC_VOICEDATA:
                p_msg = new SVC_VoiceData(buf);
                break;
            case NetMsgType::SVC_SOUNDS:
                p_msg = new SVC_Sounds(buf);
                break;
            case NetMsgType::SVC_SETVIEW:
                p_msg = new SVC_SetView(buf);
                break;
            case NetMsgType::SVC_FIXANGLE:
                p_msg = new SVC_FixAngle(buf);
                break;
            case NetMsgType::SVC_CROSSHAIRANGLE:
                p_msg = new SVC_CrosshairAngle(buf);
                break;
            case NetMsgType::SVC_BSPDECAL:
                p_msg = new SVC_BSPDecal(buf);
                break;
            case NetMsgType::SVC_USERMESSAGE:
                p_msg = new SVC_UserMessage(buf);
                break;
            case NetMsgType::SVC_ENTITYMESSAGE:
                p_msg = new SVC_EntityMessage(buf);
                break;
            case NetMsgType::SVC_GAMEEVENT:
                p_msg = new SVC_GameEvent(buf);
                break;
            case NetMsgType::SVC_PACKETENTITIES:
                p_msg = new SVC_PacketEntities(buf);
                break;
            case NetMsgType::SVC_TEMPENTITIES:
                p_msg = new SVC_TempEntities(buf);
                break;
            case NetMsgType::SVC_PREFETCH:
                p_msg = new SVC_Prefetch(buf);
                break;
            case NetMsgType::SVC_MENU:
                p_msg = new SVC_Menu(buf);
                break;
            case NetMsgType::SVC_GAMEEVENTLIST:
                p_msg = new SVC_GameEventList(buf);
                break;
            case NetMsgType::SVC_GETCVARVALUE:
                p_msg = new SVC_GetCvarValue(buf);
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
