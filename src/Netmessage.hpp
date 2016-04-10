#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <utility>

#include "BitBuffer.hpp"

enum class NetMsgType
{
    NET_NOP = 0,
    NET_DISCONNECT = 1,
    NET_FILE = 2,
    NET_TICK = 3,
    NET_STRINGCMD = 4,
    NET_SETCONVAR = 5,
    NET_SIGNONSTATE = 6,
    SVC_PRINT = 7,
    SVC_SERVERINFO = 8,
    SVC_SENDTABLE = 9,
    SVC_CLASSINFO = 10,
    SVC_SETPAUSE = 11,
    SVC_CREATESTRINGTABLE = 12,
    SVC_UPDATESTRINGTABLE = 13,
    SVC_VOICEINIT = 14,
    SVC_VOICEDATA = 15,
    SVC_SOUNDS = 17,
    SVC_SETVIEW = 18,
    SVC_FIXANGLE = 19,
    SVC_CROSSHAIRANGLE = 20,
    SVC_BSPDECAL = 21,
    SVC_USERMESSAGE = 23,
    SVC_ENTITYMESSAGE = 24,
    SVC_GAMEEVENT = 25,
    SVC_PACKETENTITIES = 26,
    SVC_TEMPENTITIES = 27,
    SVC_PREFETCH = 28,
    SVC_MENU = 29,
    SVC_GAMEEVENTLIST = 30,
    SVC_GETCVARVALUE = 31
};

enum class DialogType
{
    DIALOG_MSG = 0,
    DIALOG_MENU,
    DIALOG_TEXT,
    DIALOG_ENTRY,
    DIALOG_ASKCONNECT
};

typedef std::pair<std::string,std::string> strpair;

class NetMsg
{
public:
    NetMsg(const NetMsgType& p_type);
    virtual std::string toString() const;

    static const char* name;
    NetMsgType type;
};

class NET_Nop : public NetMsg
{
public:
    NET_Nop(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
};

class NET_Disconnect : public NetMsg
{
public:
    NET_Disconnect(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    std::string reason;
};

class NET_File : public NetMsg
{
public:
    NET_File(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    uint32_t transfer_id;
    std::string filename;
    bool requested;
};

class NET_Tick : public NetMsg
{
public:
    NET_Tick(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    int32_t tick;
    uint16_t host_frametime;
    uint16_t host_ftime_stddev;
};

class NET_StringCmd : public NetMsg
{
public:
    NET_StringCmd(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    std::string command;
};

class NET_SetConVar : public NetMsg
{
public:
    NET_SetConVar(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    std::vector<strpair> vars;
};

class NET_SignonState : public NetMsg
{
public:
    NET_SignonState(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    uint8_t signon_state;
    int32_t spawn_count;
};

class SVC_Print : public NetMsg
{
public:
    SVC_Print(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    std::string text;
};

class SVC_ServerInfo : public NetMsg
{
public:
    SVC_ServerInfo(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    int16_t protocol;
    int32_t server_count;
    bool is_sourcetv;
    bool is_dedicated;
    uint32_t client_crc;
    int16_t max_classes;
    uint32_t map_crc;
    std::vector<char> map_md5;
    uint8_t current_players;
    uint8_t max_players;
    float tick_interval;
    int8_t platform;
    std::string game_dir;
    std::string map_name;
    std::string sky_name;
    std::string host_name;
    bool has_replay;
};

class SVC_SendTable : public NetMsg
{
public:
    SVC_SendTable(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    bool needs_decoder;
    size_t length;
    std::vector<char> data;
};

class SVC_ClassInfo : public NetMsg
{
public:
    SVC_ClassInfo(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    size_t num_classes;
    bool create_on_client;
    std::vector<ClassInfo> classes;
};

class SVC_SetPause : public NetMsg
{
public:
    SVC_SetPause(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    bool pause;
};

class SVC_CreateStringTable : public NetMsg
{
public:
    SVC_CreateStringTable(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    std::string tablename;
    uint16_t max_entries;
    int32_t num_entries;
    bool userdata_fixed_size;
    uint16_t userdata_size;
    uint8_t userdata_size_bits;
    bool is_compressed;
    size_t length;
    std::vector<char> data;
};

class SVC_UpdateStringTable : public NetMsg
{
public:
    SVC_UpdateStringTable(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    int32_t table_id;
    uint16_t changed_entries;
    size_t length;
    std::vector<char> data;
};

class SVC_VoiceInit : public NetMsg
{
public:
    SVC_VoiceInit(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    std::string codec_name;
    uint8_t quality;
};

class SVC_VoiceData : public NetMsg
{
public:
    SVC_VoiceData(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    uint8_t from_client;
    bool proximity;
    size_t length;
    std::vector<char> data;
};

class SVC_Sounds : public NetMsg
{
public:
    SVC_Sounds(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    bool reliable_sound;
    uint8_t num_sounds;
    size_t length;
    std::vector<char> data;
};

class SVC_SetView : public NetMsg
{
public:
    SVC_SetView(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    uint16_t entity_index;
};

class SVC_FixAngle : public NetMsg
{
public:
    SVC_FixAngle(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    bool relative;
    QAngle angle;
};

class SVC_CrosshairAngle : public NetMsg
{
public:
    SVC_CrosshairAngle(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    QAngle angle;
};

class SVC_BSPDecal : public NetMsg
{
public:
    SVC_BSPDecal(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    VecCoord pos;
    uint16_t decal_texture_index;
    uint16_t entity_index;
    uint16_t model_index;
    bool low_priority;
};

class SVC_UserMessage : public NetMsg
{
public:
    SVC_UserMessage(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    uint8_t msg_type;
    size_t length;
    std::vector<char> data;
};

class SVC_EntityMessage : public NetMsg
{
public:
    SVC_EntityMessage(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    uint16_t entity_index;
    uint16_t class_id;
    size_t length;
    std::vector<char> data;
};

class SVC_GameEvent : public NetMsg
{
public:
    SVC_GameEvent(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    size_t length;
    std::vector<char> data;
};

class SVC_PacketEntities : public NetMsg
{
public:
    SVC_PacketEntities(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    size_t max_entries;
    size_t updated_entries;
    bool is_delta;
    int32_t delta_from;
    bool baseline;
    bool update_baseline;
    size_t length;
    std::vector<char> data;
};

class SVC_TempEntities : public NetMsg
{
public:
    SVC_TempEntities(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    uint8_t num_entries;
    size_t length;
    std::vector<char> data;
};

class SVC_Prefetch : public NetMsg
{
public:
    SVC_Prefetch(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    uint16_t type;
    uint16_t sound_index;
};

class SVC_Menu : public NetMsg
{
public:
    SVC_Menu(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;

    DialogType type;
    size_t length;
    std::vector<char> data;
};

class SVC_GameEventList : public NetMsg
{
public:
    SVC_GameEventList(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    uint16_t num_events;
    size_t length;
    std::vector<char> data;
};

class SVC_GetCvarValue : public NetMsg
{
public:
    SVC_GetCvarValue(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
    int32_t cookie;
    std::string cvar_name;
};

class SVC_CmdKeyValues : public NetMsg
{
public:
    SVC_CmdKeyValues(BitBuffer& buf);
    virtual std::string toString() const;

    static const char* name;
};
