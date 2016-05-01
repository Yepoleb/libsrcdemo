#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <utility>

#include "eventparser.hpp"

class BitBuffer;
class STableEntry;

// SVC_CreateStringTable
const size_t MAX_USERDATA_BITS = 14;
const size_t SUBSTRING_BITS = 5;
const size_t NET_MAX_PALYLOAD_BITS = 17;

// SVC_Prefetch
const size_t MAX_SOUND_INDEX_BITS = 14;

// SVC_TempEntities
const size_t EVENT_INDEX_BITS = 8;

// SVC_UpdateStringTable
const size_t MAX_TABLES = 32;

// SVC_EntityMessage
const size_t MAX_SERVER_CLASS_BITS = 9;

// SVC_BSPDecal
const size_t MAX_DECAL_INDEX_BITS = 9;
const size_t SP_MODEL_INDEX_BITS = 13;

// SVC_PacketEntities
const size_t DELTASIZE_BITS = 20;

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

inline uint32_t SwapU32(uint32_t num)
{
    uint32_t swapped;
    swapped  = (num & 0xFF000000) >> 24;
    swapped |= (num & 0x00FF0000) >>  8;
    swapped |= (num & 0x0000FF00) <<  8;
    swapped |= (num & 0x000000FF) << 24;
    return swapped;
}



class NetMsg
{
public:
    virtual ~NetMsg() = default;
    virtual std::string toString() const = 0;
    virtual std::string getName() const = 0;
    virtual NetMsgType getType() const = 0;
};

class NET_Nop : public NetMsg
{
public:
    NET_Nop(BitBuffer& buf);
    virtual ~NET_Nop() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "NET_Nop"; };
    virtual NetMsgType getType() const { return NetMsgType::NET_NOP; };
};

class NET_Disconnect : public NetMsg
{
public:
    NET_Disconnect(BitBuffer& buf);
    virtual ~NET_Disconnect() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "NET_Disconnect"; };
    virtual NetMsgType getType() const { return NetMsgType::NET_DISCONNECT; };

    std::string reason;
};

class NET_File : public NetMsg
{
public:
    NET_File(BitBuffer& buf);
    virtual ~NET_File() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "NET_File"; };
    virtual NetMsgType getType() const { return NetMsgType::NET_FILE; };

    uint32_t transfer_id;
    std::string filename;
    bool requested;
};

class NET_Tick : public NetMsg
{
public:
    NET_Tick(BitBuffer& buf);
    virtual ~NET_Tick() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "NET_Tick"; };
    virtual NetMsgType getType() const { return NetMsgType::NET_TICK; };

    int32_t tick;
    uint16_t host_frametime;
    uint16_t host_ftime_stddev;
};

class NET_StringCmd : public NetMsg
{
public:
    NET_StringCmd(BitBuffer& buf);
    virtual ~NET_StringCmd() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "NET_StringCmd"; };
    virtual NetMsgType getType() const { return NetMsgType::NET_STRINGCMD; };

    std::string command;
};

class NET_SetConVar : public NetMsg
{
public:
    NET_SetConVar(BitBuffer& buf);
    virtual ~NET_SetConVar() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "NET_SetConVar"; };
    virtual NetMsgType getType() const { return NetMsgType::NET_SETCONVAR; };

    std::vector<strpair> vars;
};

class NET_SignonState : public NetMsg
{
public:
    NET_SignonState(BitBuffer& buf);
    virtual ~NET_SignonState() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "NET_SignonState"; };
    virtual NetMsgType getType() const { return NetMsgType::NET_SIGNONSTATE; };

    uint8_t signon_state;
    int32_t spawn_count;
};

class SVC_Print : public NetMsg
{
public:
    SVC_Print(BitBuffer& buf);
    virtual ~SVC_Print() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_Print"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_PRINT; };

    std::string text;
};

class SVC_ServerInfo : public NetMsg
{
public:
    SVC_ServerInfo(BitBuffer& buf);
    virtual ~SVC_ServerInfo() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_ServerInfo"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_SERVERINFO; };

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
    virtual ~SVC_SendTable() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_SendTable"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_SENDTABLE; };

    bool needs_decoder;
    size_t length;
    std::vector<char> data;
};

class SVC_ClassInfo : public NetMsg
{
public:
    SVC_ClassInfo(BitBuffer& buf);
    virtual ~SVC_ClassInfo() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_ClassInfo"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_CLASSINFO; };

    size_t num_classes;
    bool create_on_client;
    std::vector<ClassInfo> classes;
};

class SVC_SetPause : public NetMsg
{
public:
    SVC_SetPause(BitBuffer& buf);
    virtual ~SVC_SetPause() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_SetPause"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_SETPAUSE; };

    bool pause;
};

class SVC_CreateStringTable : public NetMsg
{
public:
    SVC_CreateStringTable(BitBuffer& buf);
    virtual ~SVC_CreateStringTable() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_CreateStringTable"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_CREATESTRINGTABLE; };

    std::string tablename;
    uint16_t max_entries;
    int32_t num_entries;
    bool userdata_fixed_size;
    uint16_t userdata_size;
    uint8_t userdata_size_bits;
    bool is_compressed;
    std::vector<STableEntry> entries;
};

class SVC_UpdateStringTable : public NetMsg
{
public:
    SVC_UpdateStringTable(BitBuffer& buf);
    virtual ~SVC_UpdateStringTable() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_UpdateStringTable"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_UPDATESTRINGTABLE; };

    int32_t table_id;
    uint16_t changed_entries;
    size_t length;
    std::vector<char> data;
};

class SVC_VoiceInit : public NetMsg
{
public:
    SVC_VoiceInit(BitBuffer& buf);
    virtual ~SVC_VoiceInit() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_VoiceInit"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_VOICEINIT; };

    std::string codec_name;
    uint8_t quality;
};

class SVC_VoiceData : public NetMsg
{
public:
    SVC_VoiceData(BitBuffer& buf);
    virtual ~SVC_VoiceData() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_VoiceData"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_VOICEDATA; };

    uint8_t from_client;
    bool proximity;
    size_t length;
    std::vector<char> data;
};

class SVC_Sounds : public NetMsg
{
public:
    SVC_Sounds(BitBuffer& buf);
    virtual ~SVC_Sounds() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_Sounds"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_SOUNDS; };

    bool reliable_sound;
    uint8_t num_sounds;
    size_t length;
    std::vector<char> data;
};

class SVC_SetView : public NetMsg
{
public:
    SVC_SetView(BitBuffer& buf);
    virtual ~SVC_SetView() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_SetView"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_SETVIEW; };

    uint16_t entity_index;
};

class SVC_FixAngle : public NetMsg
{
public:
    SVC_FixAngle(BitBuffer& buf);
    virtual ~SVC_FixAngle() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_FixAngle"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_FIXANGLE; };

    bool relative;
    QAngle angle;
};

class SVC_CrosshairAngle : public NetMsg
{
public:
    SVC_CrosshairAngle(BitBuffer& buf);
    virtual ~SVC_CrosshairAngle() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_CrosshairAngle"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_CROSSHAIRANGLE; };

    QAngle angle;
};

class SVC_BSPDecal : public NetMsg
{
public:
    SVC_BSPDecal(BitBuffer& buf);
    virtual ~SVC_BSPDecal() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_BSPDecal"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_BSPDECAL; };

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
    virtual ~SVC_UserMessage() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_UserMessage"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_USERMESSAGE; };

    uint8_t msg_type;
    size_t length;
    std::vector<char> data;
};

class SVC_EntityMessage : public NetMsg
{
public:
    SVC_EntityMessage(BitBuffer& buf);
    virtual ~SVC_EntityMessage() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_EntityMessage"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_ENTITYMESSAGE; };

    uint16_t entity_index;
    uint16_t class_id;
    size_t length;
    std::vector<char> data;
};

class SVC_GameEvent : public NetMsg
{
public:
    SVC_GameEvent(BitBuffer& buf);
    virtual ~SVC_GameEvent() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_GameEvent"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_GAMEEVENT; };

    Event event;
    size_t length;
    std::vector<char> data;
};

class SVC_PacketEntities : public NetMsg
{
public:
    SVC_PacketEntities(BitBuffer& buf);
    virtual ~SVC_PacketEntities() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_PacketEntities"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_PACKETENTITIES; };

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
    virtual ~SVC_TempEntities() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_TempEntities"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_TEMPENTITIES; };

    uint8_t num_entries;
    size_t length;
    std::vector<char> data;
};

class SVC_Prefetch : public NetMsg
{
public:
    SVC_Prefetch(BitBuffer& buf);
    virtual ~SVC_Prefetch() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_Prefetch"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_PREFETCH; };

    uint16_t fetch_type;
    uint16_t sound_index;
};

class SVC_Menu : public NetMsg
{
public:
    SVC_Menu(BitBuffer& buf);
    virtual ~SVC_Menu() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_Menu"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_MENU; };

    DialogType dialog_type;
    size_t length;
    std::vector<char> data;
};

class SVC_GameEventList : public NetMsg
{
public:
    SVC_GameEventList(BitBuffer& buf);
    virtual ~SVC_GameEventList() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_GameEventList"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_GAMEEVENTLIST; };

    size_t length;
    std::vector<char> data;
    uint16_t num_events;
    std::vector<EventDescriptor> descriptors;
};

class SVC_GetCvarValue : public NetMsg
{
public:
    SVC_GetCvarValue(BitBuffer& buf);
    virtual ~SVC_GetCvarValue() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SVC_GetCvarValue"; };
    virtual NetMsgType getType() const { return NetMsgType::SVC_GETCVARVALUE; };

    int32_t cookie;
    std::string cvar_name;
};
