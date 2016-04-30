#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include "snappy.h"

#include "bitbuffer.hpp"
#include "eventparser.hpp"
#include "demmessages/stringtables.hpp"
#include "helpers.hpp"
#include "common.hpp"
#include "netmessage.hpp"

const char* NetMsg::name = "Unknown Packet";
const char* NET_Nop::name = "NET_Nop";
const char* NET_Disconnect::name = "NET_Disconnect";
const char* NET_File::name = "NET_File";
const char* NET_Tick::name = "NET_Tick";
const char* NET_StringCmd::name = "NET_StringCmd";
const char* NET_SetConVar::name = "NET_SetConVar";
const char* NET_SignonState::name = "NET_SignonState";
const char* SVC_Print::name = "SVC_Print";
const char* SVC_ServerInfo::name = "SVC_ServerInfo";
const char* SVC_SendTable::name = "SVC_SendTable";
const char* SVC_ClassInfo::name = "SVC_ClassInfo";
const char* SVC_SetPause::name = "SVC_SetPause";
const char* SVC_CreateStringTable::name = "SVC_CreateStringTable";
const char* SVC_UpdateStringTable::name = "SVC_UpdateStringTable";
const char* SVC_VoiceInit::name = "SVC_VoiceInit";
const char* SVC_VoiceData::name = "SVC_VoiceData";
const char* SVC_Sounds::name = "SVC_Sounds";
const char* SVC_SetView::name = "SVC_SetView";
const char* SVC_FixAngle::name = "SVC_FixAngle";
const char* SVC_CrosshairAngle::name = "SVC_CrosshairAngle";
const char* SVC_BSPDecal::name = "SVC_BSPDecal";
const char* SVC_UserMessage::name = "SVC_UserMessage";
const char* SVC_EntityMessage::name = "SVC_EntityMessage";
const char* SVC_GameEvent::name = "SVC_GameEvent";
const char* SVC_PacketEntities::name = "SVC_PacketEntities";
const char* SVC_TempEntities::name = "SVC_TempEntities";
const char* SVC_Prefetch::name = "SVC_Prefetch";
const char* SVC_Menu::name = "SVC_Menu";
const char* SVC_GameEventList::name = "SVC_GameEventList";
const char* SVC_GetCvarValue::name = "SVC_GetCvarValue";

const uint32_t LZSS_MAGIC = SwapU32(('L' << 24) | ('Z' << 16) | ('S' << 8) | ('S'));
const uint32_t SNAPPY_MAGIC = SwapU32(('S' << 24) | ('N' << 16) | ('A' << 8) | ('P'));

NET_Nop::NET_Nop(BitBuffer& buf)
{ }

std::string NET_Nop::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    return ss.str();
}

NET_Disconnect::NET_Disconnect(BitBuffer& buf)
{
    reason = buf.ReadString();
}

std::string NET_Disconnect::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  reason: " << reason << std::endl;
    return ss.str();
}

NET_File::NET_File(BitBuffer& buf)
{
    transfer_id = buf.ReadU32();
    filename = buf.ReadString();
    requested = buf.ReadBool();
}

std::string NET_File::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << std::boolalpha;
    ss << "  transfer_id: " << transfer_id << std::endl;
    ss << "  filename: " << filename << std::endl;
    ss << "  requested: " << requested << std::endl;
    return ss.str();
}

NET_Tick::NET_Tick(BitBuffer& buf)
{
    tick = buf.ReadS32();
    host_frametime = buf.ReadU16();
    host_ftime_stddev = buf.ReadU16();
#ifdef DEBUG_TICKS
    std::cout << "tick: " << tick << std::endl;
#endif
}

std::string NET_Tick::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  tick: " << tick << std::endl;
    ss << "  host_frametime: " << host_frametime << std::endl;
    ss << "  host_ftime_stddev: " << host_ftime_stddev << std::endl;
    return ss.str();
}

NET_StringCmd::NET_StringCmd(BitBuffer& buf)
{
    command = buf.ReadString();
}

std::string NET_StringCmd::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  command: " << command << std::endl;
    return ss.str();
}

NET_SetConVar::NET_SetConVar(BitBuffer& buf)
{
    uint8_t num = buf.ReadU8();
    for (uint8_t i = 0; i < num; i++) {
        std::string key = buf.ReadString();
        std::string value = buf.ReadString();
        vars.push_back(strpair(key, value));
    }
}

std::string NET_SetConVar::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    for (const strpair& pair : vars) {
        ss << "  " << pair.first << ": " << pair.second << std::endl;
    }
    return ss.str();
}

NET_SignonState::NET_SignonState(BitBuffer& buf)
{
    signon_state = buf.ReadU8();
    spawn_count = buf.ReadS32();
}

std::string NET_SignonState::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  signon_state: " << (int)signon_state << std::endl;
    ss << "  spawn_count: " << spawn_count << std::endl;
    return ss.str();
}

SVC_Print::SVC_Print(BitBuffer& buf)
{
    text = buf.ReadString();
}

std::string SVC_Print::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  text: " << text << std::endl;
    return ss.str();
}

SVC_ServerInfo::SVC_ServerInfo(BitBuffer& buf)
{
    protocol = buf.ReadS16();
    server_count = buf.ReadS32();
    is_sourcetv = buf.ReadBool();
    is_dedicated = buf.ReadBool();
    client_crc = buf.ReadU32();
    max_classes = buf.ReadU16();
    if (protocol < 18) {
        map_crc = buf.ReadU32();
    } else {
        map_md5 = buf.ReadData(128);
    }
    current_players = buf.ReadU8();
    max_players = buf.ReadU8();
    tick_interval = buf.ReadFloat();
    platform = buf.ReadS8();
    game_dir = buf.ReadString();
    map_name = buf.ReadString();
    sky_name = buf.ReadString();
    host_name = buf.ReadString();
    if (protocol > 15) {
        has_replay = buf.ReadBool();
    }
}

std::string SVC_ServerInfo::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << std::boolalpha;
    ss << "  protocol: " << protocol << std::endl;
    ss << "  server_count: " << server_count << std::endl;
    ss << "  is_sourcetv: " << is_sourcetv << std::endl;
    ss << "  is_dedicated: " << is_dedicated << std::endl;
    ss << "  client_crc: " << std::hex << client_crc << std::dec << std::endl;
    ss << "  max_classes: " << max_classes << std::endl;
    if (protocol < 18) {
        ss << "  map_crc: " << std::hex << map_crc << std::dec << std::endl;
    } else {
        ss << "  map_md5: " << bindump(map_md5, false) << std::endl;
    }
    ss << "  current_players: " << (int)current_players << std::endl;
    ss << "  max_players: " << (int)max_players << std::endl;
    ss << "  tick_interval: " << tick_interval << std::endl;
    ss << "  platform: " << platform << std::endl;
    ss << "  game_dir: " << game_dir << std::endl;
    ss << "  map_name: " << map_name << std::endl;
    ss << "  sky_name: " << sky_name << std::endl;
    ss << "  host_name: " << host_name << std::endl;
    if (protocol > 15) {
        ss << "  has_replay: " << has_replay << std::endl;
    }
    return ss.str();
}

SVC_SendTable::SVC_SendTable(BitBuffer& buf)
{
    needs_decoder = buf.ReadBool();
    length = buf.ReadS16();
    data = buf.ReadData(length);
}

std::string SVC_SendTable::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << std::boolalpha;
    ss << "  needs_decoder: " << needs_decoder << std::endl;
    ss << "  length: " << length << std::endl;
    ss << format_data(data) << std::endl;
    return ss.str();
}

SVC_ClassInfo::SVC_ClassInfo(BitBuffer& buf)
{
    num_classes = buf.ReadS16();
    size_t class_bits = std::log2(num_classes) + 1;
    create_on_client = buf.ReadBool();
    if (!create_on_client) {
        for (size_t i = 0; i < num_classes; i++) {
            ClassInfo class_info;
            class_info.class_id = buf.ReadBits(class_bits);
            class_info.classname = buf.ReadString();
            class_info.tablename = buf.ReadString();
            classes.push_back(class_info);
        }
    }
}

std::string SVC_ClassInfo::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << std::boolalpha;
    ss << "  num_classes: " << num_classes << std::endl;
    ss << "  create_on_client: " << create_on_client << std::endl;
    if (!create_on_client) {
        ss << "  classes:" << std::endl;
        for (const ClassInfo& class_info : classes) {
            ss << "    class_id: " << class_info.class_id << std::endl;
            ss << "    classname: " << class_info.classname << std::endl;
            ss << "    tablename: " << class_info.tablename << std::endl;
            ss << std::endl;
        }
    }
    return ss.str();
}

SVC_SetPause::SVC_SetPause(BitBuffer& buf)
{
    pause = buf.ReadBool();
}

std::string SVC_SetPause::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << std::boolalpha;
    ss << "  pause: " << pause << std::endl;
    return ss.str();
}

SVC_CreateStringTable::SVC_CreateStringTable(BitBuffer& buf)
{
    tablename = buf.ReadString();
    max_entries = buf.ReadU16();
    size_t encode_bits = std::log2(max_entries);
    num_entries = buf.ReadBits(encode_bits + 1);
    size_t length;
    if (NETPROTOCOL_VERSION >= 24) {
        length = buf.ReadVarU32();
    } else {
        length = buf.ReadBits(NET_MAX_PALYLOAD_BITS + 3);
    }

    userdata_fixed_size = buf.ReadBool();
    if (userdata_fixed_size) {
        userdata_size = buf.ReadBits(12);
        userdata_size_bits = buf.ReadBits(4);
    } else {
        userdata_size = 0;
        userdata_size_bits = 0;
    }

    if (NETPROTOCOL_VERSION >= 15) {
        is_compressed = buf.ReadBool();
    }

    std::vector<char> data = buf.ReadData(length);

    BitBuffer table_buf(data.data(), data.size());
    std::vector<char> decompressed_data;
    if (is_compressed) {
        size_t decompressed_size = table_buf.ReadU32();
        size_t compressed_size = table_buf.ReadU32() - 4;
        uint32_t magic = table_buf.ReadU32();
        std::vector<char> compressed_data;
        decompressed_data.resize(decompressed_size);
        compressed_data = table_buf.ReadData(compressed_size * 8);
        if (magic == LZSS_MAGIC) {
            std::cerr << "LZSS compression is not supported" << std::endl;
            throw std::exception();
        } else if (magic == SNAPPY_MAGIC) {
            snappy::RawUncompress(compressed_data.data(), compressed_size, decompressed_data.data());
        } else {
            std::cerr << "Unsupported compression " << magic << std::endl;
            throw std::exception();
        }
        table_buf = BitBuffer(decompressed_data.data(), decompressed_size);
    }

    std::vector<std::string> history;
    size_t entry_index = 0;
    for (int32_t i = 0; i < num_entries; i++) {
        if (not table_buf.ReadBool()) {
            entry_index = table_buf.ReadBits(encode_bits);
        }

        STableEntry entry;
        entry.index = entry_index;
        if (table_buf.ReadBool()) {
            bool has_basestring = table_buf.ReadBool();
            if (has_basestring) {
                uint8_t substr_index = table_buf.ReadBits(5);
                uint8_t substr_length = table_buf.ReadBits(SUBSTRING_BITS);
                std::string basestring = history.at(substr_index);
                std::string substring = basestring.substr(0, substr_length);
                entry.name = substring + table_buf.ReadString();
            } else {
                entry.name = table_buf.ReadString();
            }
        }

        if (table_buf.ReadBool()) {
            if (userdata_fixed_size) {
                entry.length = userdata_size_bits;
                entry.data = table_buf.ReadData(userdata_size_bits);
            } else {
                size_t userdata_size = table_buf.ReadBits(MAX_USERDATA_BITS) * 8;
                entry.length = userdata_size;
                entry.data = table_buf.ReadData(userdata_size);
            }
        } else {
            entry.length = 0;
        }

        if (history.size() >= 32) {
            history.erase(history.begin());
        }

        entries.push_back(entry);
        history.push_back(entry.name);

        entry_index++;
    }
}

std::string SVC_CreateStringTable::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << std::boolalpha;
    ss << "  tablename: " << tablename << std::endl;
    ss << "  max_entries: " << max_entries << std::endl;
    ss << "  num_entries: " << num_entries << std::endl;
    ss << "  userdata_fixed_size: " << userdata_fixed_size << std::endl;
    ss << "  userdata_size: " << userdata_size << std::endl;
    ss << "  userdata_size_bits: " << (int)userdata_size_bits << std::endl;
    ss << "  is_compressed: " << is_compressed << std::endl;
    //ss << "  length: " << length << std::endl;
    //ss << format_data(data) << std::endl;
    for (STableEntry entry : entries) {
        ss << indent(entry.toString(), 2);
    }
    return ss.str();
}

SVC_UpdateStringTable::SVC_UpdateStringTable(BitBuffer& buf)
{
    table_id = buf.ReadBits(std::log2(MAX_TABLES));
    if (buf.ReadBool()) {
        changed_entries = buf.ReadU16();
    } else {
        changed_entries = 1;
    }

    length = buf.ReadBits(20);
    data = buf.ReadData(length);
}

std::string SVC_UpdateStringTable::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  table_id: " << table_id << std::endl;
    ss << "  changed_entries: " << changed_entries << std::endl;
    ss << "  length: " << length << std::endl;
    ss << format_data(data) << std::endl;
    return ss.str();
}

SVC_VoiceInit::SVC_VoiceInit(BitBuffer& buf)
{
    codec_name = buf.ReadString();
    quality = buf.ReadU8();
}

std::string SVC_VoiceInit::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  codec_name: " << codec_name << std::endl;
    ss << "  quality: " << (int)quality << std::endl;
    return ss.str();
}

SVC_VoiceData::SVC_VoiceData(BitBuffer& buf)
{
    from_client = buf.ReadU8();
    proximity = buf.ReadU8();
    length = buf.ReadU16();
    data = buf.ReadData(length);
}

std::string SVC_VoiceData::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  from_client: " << from_client << std::endl;
    ss << "  proximity: " << proximity << std::endl;
    ss << "  length: " << length << std::endl;
    ss << format_data(data) << std::endl;
    return ss.str();
}

SVC_Sounds::SVC_Sounds(BitBuffer& buf)
{
    reliable_sound = buf.ReadBool();
    if (reliable_sound) {
        num_sounds = 1;
        length = buf.ReadU8();
    } else {
        num_sounds = buf.ReadU8();
        length = buf.ReadU16();
    }

    data = buf.ReadData(length);
    // public/soundinfo.h SoundInfo_t::ReadDelta
    // engine/servermsghandler.cpp CClientState::ProcessSounds
}

std::string SVC_Sounds::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << std::boolalpha;
    ss << "  reliable_sound: " << reliable_sound << std::endl;
    ss << "  num_sounds: " << (int)num_sounds << std::endl;
    ss << "  length: " << length << std::endl;
    ss << format_data(data) << std::endl;
    return ss.str();
}

SVC_SetView::SVC_SetView(BitBuffer& buf)
{
    entity_index = buf.ReadBits(MAX_EDICT_BITS);
}

std::string SVC_SetView::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  entity_index: " << entity_index << std::endl;
    return ss.str();
}

SVC_FixAngle::SVC_FixAngle(BitBuffer& buf)
{
    relative = buf.ReadBool();
    angle.x = buf.ReadBitAngle(16);
    angle.y = buf.ReadBitAngle(16);
    angle.z = buf.ReadBitAngle(16);
}

std::string SVC_FixAngle::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << std::boolalpha;
    ss << "  relative: " << relative << std::endl;
    ss << "  angle.x: " << angle.x << std::endl;
    ss << "  angle.y: " << angle.y << std::endl;
    ss << "  angle.z: " << angle.z << std::endl;
    return ss.str();
}

SVC_CrosshairAngle::SVC_CrosshairAngle(BitBuffer& buf)
{
    angle.x = buf.ReadBitAngle(16);
    angle.y = buf.ReadBitAngle(16);
    angle.z = buf.ReadBitAngle(16);
}

std::string SVC_CrosshairAngle::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  angle.x: " << angle.x << std::endl;
    ss << "  angle.y: " << angle.y << std::endl;
    ss << "  angle.z: " << angle.z << std::endl;
    return ss.str();
}

SVC_BSPDecal::SVC_BSPDecal(BitBuffer& buf)
{
    pos = buf.ReadVecCoord();
    decal_texture_index = buf.ReadBits(MAX_DECAL_INDEX_BITS);
    if (buf.ReadBool()) {
        entity_index = buf.ReadBits(MAX_EDICT_BITS);
        model_index = buf.ReadBits(SP_MODEL_INDEX_BITS);
    } else {
        entity_index = 0;
        model_index = 0;
    }
    low_priority = buf.ReadBool();
}

std::string SVC_BSPDecal::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << std::boolalpha;
    ss << "  pos.x: " << pos.x << std::endl;
    ss << "  pos.y: " << pos.y << std::endl;
    ss << "  pos.z: " << pos.z << std::endl;
    ss << "  decal_texture_index: " << decal_texture_index << std::endl;
    ss << "  entity_index: " << entity_index << std::endl;
    ss << "  model_index: " << model_index << std::endl;
    ss << "  low_priority: " << low_priority << std::endl;
    return ss.str();
}

SVC_UserMessage::SVC_UserMessage(BitBuffer& buf)
{
    msg_type = buf.ReadU8();
    length = buf.ReadBits(11);
    data = buf.ReadData(length);
}

std::string SVC_UserMessage::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  msg_type: " << (int)msg_type << std::endl;
    ss << "  length: " << length << std::endl;
    ss << format_data(data) << std::endl;
    return ss.str();
}

SVC_EntityMessage::SVC_EntityMessage(BitBuffer& buf)
{
    entity_index = buf.ReadBits(MAX_EDICT_BITS);
    class_id = buf.ReadBits(MAX_SERVER_CLASS_BITS);
    length = buf.ReadBits(11);
    data = buf.ReadData(length);
}

std::string SVC_EntityMessage::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  entity_index: " << entity_index << std::endl;
    ss << "  class_id: " << class_id << std::endl;
    ss << "  length: " << length << std::endl;
    ss << format_data(data) << std::endl;
    return ss.str();
}

SVC_GameEvent::SVC_GameEvent(BitBuffer& buf)
{
    length = buf.ReadBits(11);
    data = buf.ReadData(length);
    BitBuffer evt_buf(data.data(), data.size());
    event = g_evtparser->parseEvent(evt_buf);
}

std::string SVC_GameEvent::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  event:\n" << indent(event.toString(), 4);
    return ss.str();
}

SVC_PacketEntities::SVC_PacketEntities(BitBuffer& buf)
{
    max_entries = buf.ReadBits(MAX_EDICT_BITS);
    is_delta = buf.ReadBool();
    if (is_delta) {
        delta_from = buf.ReadS32();
    } else {
        delta_from = -1;
    }
    baseline = buf.ReadBool();
    updated_entries = buf.ReadBits(MAX_EDICT_BITS);
    length = buf.ReadBits(DELTASIZE_BITS);
    update_baseline = buf.ReadBool();
    data = buf.ReadData(length);
}

std::string SVC_PacketEntities::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << std::boolalpha;
    ss << "  max_entries: " << max_entries << std::endl;
    ss << "  updated_entries: " << updated_entries << std::endl;
    ss << "  is_delta: " << is_delta << std::endl;
    if (is_delta) {
        ss << "  delta_from: " << delta_from << std::endl;
    }
    ss << "  baseline: " << baseline << std::endl;
    ss << "  update_baseline: " << update_baseline << std::endl;
    ss << "  length: " << length << std::endl;
    ss << format_data(data) << std::endl;
    return ss.str();
}

SVC_TempEntities::SVC_TempEntities(BitBuffer& buf)
{
    num_entries = buf.ReadBits(EVENT_INDEX_BITS);
    if (NETPROTOCOL_VERSION >= 24) {
        length = buf.ReadVarU32();
    } else {
        length = buf.ReadBits(NET_MAX_PALYLOAD_BITS);
    }
    data = buf.ReadData(length);
}

std::string SVC_TempEntities::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  num_entries: " << (int)num_entries << std::endl;
    ss << "  length: " << length << std::endl;
    ss << format_data(data) << std::endl;
    return ss.str();
}

SVC_Prefetch::SVC_Prefetch(BitBuffer& buf)
{
    fetch_type = 0; // SOUND, should be enum
    sound_index = buf.ReadBits(MAX_SOUND_INDEX_BITS);
}

std::string SVC_Prefetch::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  type: " << fetch_type << std::endl;
    ss << "  sound_index: " << sound_index << std::endl;
    return ss.str();
}

SVC_Menu::SVC_Menu(BitBuffer& buf)
{
    dialog_type = static_cast<DialogType>(buf.ReadU16());
    length = buf.ReadU16();
    // TODO: Parse KeyValues
    data = buf.ReadData(length);
}

std::string SVC_Menu::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  type: " << static_cast<int>(type) << std::endl;
    ss << "  length: " << length << std::endl;
    ss << format_data(data) << std::endl;
    return ss.str();
}

SVC_GameEventList::SVC_GameEventList(BitBuffer& buf)
{
    num_events = buf.ReadBits(MAX_EVENT_BITS);
    buf.Seek(20); // skip length

    for (size_t event_i = 0; event_i < num_events; event_i++) {
        EventDescriptor desc;
        desc.id = buf.ReadBits(MAX_EVENT_BITS);
        desc.name = buf.ReadString();
        while (true) {
            EventField field;
            field.type = static_cast<FieldType>(buf.ReadBits(3));
            if (field.type == FieldType::TYPE_LOCAL) {
                break;
            }
            field.name = buf.ReadString();
            desc.fields.push_back(field);
        }
        descriptors.push_back(desc);
        g_evtparser->addDescriptor(desc);
    }
}

std::string SVC_GameEventList::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  num_events: " << num_events << std::endl;
    for (const EventDescriptor& desc : descriptors) {
        ss << indent(desc.toString(), 2) << std::endl;
    }
    return ss.str();
}

SVC_GetCvarValue::SVC_GetCvarValue(BitBuffer& buf)
{
    cookie = buf.ReadS32();
    cvar_name = buf.ReadString();
}

std::string SVC_GetCvarValue::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  cookie: " << cookie << std::endl;
    ss << "  cvar_name: " << cvar_name << std::endl;
    return ss.str();
}
