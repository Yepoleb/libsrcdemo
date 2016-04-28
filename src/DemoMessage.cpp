#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>

#include "DemoMessage.hpp"
#include "BitBuffer.hpp"
#include "NetMessage.hpp"
#include "helpers.hpp"
#include "common.hpp"

const char* DemoMessage::name = "Unknown DemoMessage";
const char* ConsoleCmdMsg::name = "ConsoleCmd";
const char* UserCmdMsg::name = "UserCmdMsg";
const char* SyncTickMsg::name = "SyncTickMsg";
const char* DataTablesMsg::name = "DataTablesMsg";
const char* Packet::name = "Packet";
const char* Signon::name = "Signon";
const char* StringTablesMsg::name = "StringTablesMsg";

const char* SENDPROPTYPE_NAMES[] =
{
    "Int",
    "Float",
    "Vector",
    "VectorXY",
    "String",
    "Array",
    "DataTable",
    "Int64"
};

DemoMessage::DemoMessage(const int32_t& p_tick)
{
    tick = p_tick;
}

std::string DemoMessage::toString() const
{
    std::stringstream ss;
    ss << tick << " " << name << std::endl;
    ss << "  Unknown Data" << std::endl;
    return ss.str();
}

ConsoleCmdMsg::ConsoleCmdMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(tick)
{
    // Subtract 1 from data_size to remove the trailing zero bit
    command = std::string(data, data_size - 1);
}

std::string ConsoleCmdMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << name << std::endl;
    ss << "  Command: " << command << std::endl;
    return ss.str();
}

UserCmdMsg::UserCmdMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(tick)
{
    BitBuffer buf(data, data_size);
    fields = 0;

    if (buf.ReadBool()) {
        command_number = buf.ReadU32();
        fields |= USERCMD_COMMAND_NUMBER;
    }
    if (buf.ReadBool()) {
        tick_count = buf.ReadU32();
        fields |= USERCMD_TICK_COUNT;
    }
    if (buf.ReadBool()) {
        viewangles.x = buf.ReadFloat();
        fields |= USERCMD_VIEWANGLES_X;
    }
    if (buf.ReadBool()) {
        viewangles.y = buf.ReadFloat();
        fields |= USERCMD_VIEWANGLES_Y;
    }
    if (buf.ReadBool()) {
        viewangles.z = buf.ReadFloat();
        fields |= USERCMD_VIEWANGLES_Z;
    }
    if (buf.ReadBool()) {
        forwardmove = buf.ReadFloat();
        fields |= USERCMD_FORWARDMOVE;
    }
    if (buf.ReadBool()) {
        sidemove = buf.ReadFloat();
        fields |= USERCMD_SIDEMOVE;
    }
    if (buf.ReadBool()) {
        upmove = buf.ReadFloat();
        fields |= USERCMD_UPMOVE;
    }
    if (buf.ReadBool()) {
        buttons = buf.ReadU32();
        fields |= USERCMD_BUTTONS;
    }
    if (buf.ReadBool()) {
        impulse = buf.ReadU8();
        fields |= USERCMD_IMPULSE;
    }
    if (buf.ReadBool()) {
        weaponselect = buf.ReadBits(MAX_EDICT_BITS);
        fields |= USERCMD_WEAPONSELECT;
        if (buf.ReadBool()) {
            weaponsubtype = buf.ReadBits(WEAPON_SUBTYPE_BITS);
            fields |= USERCMD_WEAPONSUBTYPE;
        }
    }
    if (buf.ReadBool()) {
        mousedx = buf.ReadS16();
        fields |= USERCMD_MOUSEDX;
    }
    if (buf.ReadBool()) {
        mousedy = buf.ReadS16();
        fields |= USERCMD_MOUSEDY;
    }
}

std::string UserCmdMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << name << std::endl;
    if (fields & USERCMD_COMMAND_NUMBER)
        ss << "  Command number: " << command_number << std::endl;
    if (fields & USERCMD_TICK_COUNT)
        ss << "  Tick count: " << tick_count << std::endl;
    if (fields & USERCMD_VIEWANGLES_X)
        ss << "  Viewangle pitch: " << viewangles.x << std::endl;
    if (fields & USERCMD_VIEWANGLES_Y)
        ss << "  Viewangle yaw: " << viewangles.y << std::endl;
    if (fields & USERCMD_VIEWANGLES_Z)
        ss << "  Viewangle roll: " << viewangles.z << std::endl;
    if (fields & USERCMD_FORWARDMOVE)
        ss << "  Foward move: " << forwardmove << std::endl;
    if (fields & USERCMD_SIDEMOVE)
        ss << "  Side move: " << sidemove << std::endl;
    if (fields & USERCMD_UPMOVE)
        ss << "  Up move: " << upmove << std::endl;
    if (fields & USERCMD_BUTTONS)
        ss << "  Buttons: 0x" << std::hex << buttons << std::dec << std::endl;
    if (fields & USERCMD_IMPULSE)
        ss << "  Impulse: " << (int)impulse << std::endl;
    if (fields & USERCMD_WEAPONSELECT)
        ss << "  Weaponselect: " << weaponselect << std::endl;
    if (fields & USERCMD_WEAPONSUBTYPE)
        ss << "  Weaponsubtype: " << weaponsubtype << std::endl;
    if (fields & USERCMD_MOUSEDX)
        ss << "  Mouse DX: " << mousedx << std::endl;
    if (fields & USERCMD_MOUSEDY)
        ss << "  Mouse DY: " << mousedy << std::endl;
    return ss.str();
}

SyncTickMsg::SyncTickMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(tick)
{

}

std::string SyncTickMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << name << std::endl;
    return ss.str();
}

std::string SendProp::toString() const
{
    std::stringstream ss;
    uint16_t temp_flags = flags;
    switch (type) {
        case SendPropType::DPT_DataTable:
            ss << exclude_dt_name << " " << name;
            break;
        case SendPropType::DPT_Int:
        case SendPropType::DPT_Int64:
            if (temp_flags & SPROP_UNSIGNED) {
                if (nbits != 1) {
                    ss << "U";
                }
                // Remove unsigned flag
                temp_flags &= ~SPROP_UNSIGNED;
            }
            if (nbits == 1) {
                ss << "Bool";
            } else {
                ss << SENDPROPTYPE_NAMES[(int)type];
                if (nbits > 0) {
                    ss << (int)nbits;
                }
            }
            ss << " " << name;
            break;
        case SendPropType::DPT_Float:
        case SendPropType::DPT_Vector:
        case SendPropType::DPT_VectorXY:
            ss << SENDPROPTYPE_NAMES[(int)type];
            if (nbits > 0) {
                ss << (int)nbits;
            }
            ss << " " << name;
            if (flow_value or fhigh_value) {
                ss << ", range: " << flow_value << " - " << fhigh_value;
            }
            break;
        case SendPropType::DPT_String:
            ss << SENDPROPTYPE_NAMES[(int)type] << " ";
            ss << name;
            break;
        case SendPropType::DPT_Array:
            ss << SENDPROPTYPE_NAMES[(int)type];
            ss << "[" << num_elements << "] " << name;
            break;
        default:
            ss << "Unknown" << (int)type << " " << name;
            break;
    }

    if (temp_flags) {
        ss << ", flags: 0x" << std::hex << temp_flags << std::dec;
    }
    ss << std::endl;

    return ss.str();
}

std::string DataTable::toString() const
{
    std::stringstream ss;
    ss << std::boolalpha;
    ss << name << " (decoder: " << needs_decoder << ")" << std::endl;
    for (const SendProp& prop : props) {
        ss << indent(prop.toString(), 2);
    }
    return ss.str();
}

DataTablesMsg::DataTablesMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(tick)
{
    BitBuffer buf(data, data_size);

    size_t nflag_bits = PROPINFOBITS_FLAGS;
    if (DEMOPROTOCOL_VERSION == 2) {
        nflag_bits = 11;
    }

    while (buf.ReadBool()) {
        DataTable* table = new DataTable;
        table->needs_decoder = buf.ReadBool();
        table->name = buf.ReadString();
        uint16_t num_props = buf.ReadBits(PROPINFOBITS_NUMPROPS);
        for (size_t i_prop = 0; i_prop < num_props; i_prop++) {
            SendProp prop;
            prop.type = static_cast<SendPropType>(buf.ReadBits(PROPINFOBITS_TYPE));
            prop.name = buf.ReadString();
            prop.flags = buf.ReadBits(nflag_bits);

            bool is_exclude = prop.flags & SPROP_EXCLUDE;
            if (prop.type == SendPropType::DPT_DataTable or is_exclude) {
                prop.exclude_dt_name = buf.ReadString();
            } else if (prop.type == SendPropType::DPT_Array) {
                prop.num_elements = buf.ReadBits(PROPINFOBITS_NUMELEMENTS);
            } else {
                prop.flow_value = buf.ReadFloat();
                prop.fhigh_value = buf.ReadFloat();
                prop.nbits = buf.ReadBits(PROPINFOBITS_NUMBITS);
            }
            table->props.push_back(prop);
        }
        tables.push_back(table);
    }

    uint16_t num_classes = buf.ReadU16();
    for (size_t i_class = 0; i_class < num_classes; i_class++) {
        ClassInfo info;
        info.class_id = buf.ReadU16();
        info.classname = buf.ReadString();
        info.tablename = buf.ReadString();
        classes.push_back(info);
    }
}

DataTablesMsg::~DataTablesMsg()
{
    for (DataTable* table : tables) {
        delete table;
    }
}

std::string DataTablesMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << name << std::endl;
    ss << "  tables:" << std::endl;
    for (DataTable* table : tables) {
        ss << indent(table->toString(), 4) << std::endl;
    }
    ss << "  classes:" << std::endl;
    for (const ClassInfo& class_info : classes) {
        ss << "    class_id: " << class_info.class_id << std::endl;
        ss << "    classname: " << class_info.classname << std::endl;
        ss << "    tablename: " << class_info.tablename << std::endl;
        ss << std::endl;
    }
    return ss.str();
}

Packet::Packet(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(tick)
{
    BitBuffer buf(data, data_size);
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

Packet::~Packet()
{
    for (NetMsg* msg : messages) {
        delete msg;
    }
}

std::string Packet::toString() const
{
    std::stringstream ss;
    ss << tick << " " << name << std::endl;
    for (NetMsg* msg : messages) {
        ss << indent(msg->toString(), 2);
    }
    return ss.str();
}

Signon::Signon(const int32_t& tick, const char* data, const size_t& data_size) :
    Packet(tick, data, data_size)
{

}

std::string Signon::toString() const
{
    return Packet::toString();
}

StringTable::StringTable(BitBuffer& buf)
{
    name = buf.ReadString();
    uint16_t num_entries = buf.ReadU16();
    for (size_t i_entry = 0; i_entry < num_entries; i_entry++) {
        STableEntry entry;
        entry.fromBuffer(i_entry, buf);
        entries.push_back(entry);
    }
    has_client_entries = buf.ReadBool();
    if (has_client_entries) {
        uint16_t num_entries_client = buf.ReadU16();
        for (size_t i_entry = 0; i_entry < num_entries_client; i_entry++) {
            STableEntry entry;
            entry.fromBuffer(i_entry, buf);
            entries_client.push_back(entry);
        }
    }
}

std::string StringTable::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  entries:" << std::endl;
    for (const STableEntry& entry : entries) {
        ss << indent(entry.toString(), 2);
    }
    if (has_client_entries) {
        ss << "  entries_client:" << std::endl;
        for (const STableEntry& entry : entries) {
            ss << indent(entry.toString(), 2);
        }
    }
    return ss.str();
}

StringTablesMsg::StringTablesMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(tick)
{
#ifdef DEM_STRINGTABLES
    BitBuffer buf(data, data_size);
    uint8_t num_tables = buf.ReadU8();
    for (size_t i_table = 0; i_table < num_tables; i_table++) {
        StringTable* table = new StringTable(buf);
        tables.push_back(table);
    }
#endif
}

StringTablesMsg::~StringTablesMsg()
{
    for (StringTable* table : tables) {
        delete table;
    }
}

std::string StringTablesMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << name << std::endl;
#ifdef DEM_STRINGTABLES
    for (StringTable* table : tables) {
        ss << indent(table->toString(), 2);
    }
#else
    ss << "  Skipped StringTableMsg parsing" << std::endl;
#endif
    return ss.str();
}
