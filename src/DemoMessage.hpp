#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include "BitBuffer.hpp"
#include "Netmessage.hpp"

enum class MessageType
{
    Signon = 1,
    Packet,
    SyncTick,
    ConsoleCmd,
    UserCmd,
    DataTables,
    Stop,
    StringTables
};

enum class SendPropType
{
    DPT_Int = 0,
    DPT_Float,
    DPT_Vector,
    DPT_VectorXY,
    DPT_String,
    DPT_Array,
    DPT_DataTable,
    DPT_Int64,
    DPT_NUMPropTypes
};

// UserCmdMsg
const uint16_t USERCMD_COMMAND_NUMBER = (1 << 0);
const uint16_t USERCMD_TICK_COUNT = (1 << 1);
const uint16_t USERCMD_VIEWANGLES_X = (1 << 2);
const uint16_t USERCMD_VIEWANGLES_Y = (1 << 3);
const uint16_t USERCMD_VIEWANGLES_Z = (1 << 4);
const uint16_t USERCMD_FORWARDMOVE = (1 << 5);
const uint16_t USERCMD_SIDEMOVE = (1 << 6);
const uint16_t USERCMD_UPMOVE = (1 << 7);
const uint16_t USERCMD_BUTTONS = (1 << 8);
const uint16_t USERCMD_IMPULSE = (1 << 9);
const uint16_t USERCMD_WEAPONSELECT = (1 << 10);
const uint16_t USERCMD_WEAPONSUBTYPE = (1 << 11);
const uint16_t USERCMD_MOUSEDX = (1 << 12);
const uint16_t USERCMD_MOUSEDY = (1 << 13);
const size_t WEAPON_SUBTYPE_BITS = 6;

// SendProp
const uint16_t SPROP_UNSIGNED = (1 << 0);
const uint16_t SPROP_COORD = (1 << 1);
const uint16_t SPROP_NOSCALE = (1 << 2);
const uint16_t SPROP_ROUNDDOWN = (1 << 3);
const uint16_t SPROP_ROUNDUP = (1 << 4);
const uint16_t SPROP_NORMAL = (1 << 5);
const uint16_t SPROP_EXCLUDE = (1 << 6);
const uint16_t SPROP_XYZE = (1 << 7);
const uint16_t SPROP_INSIDEARRAY = (1 << 8);
const uint16_t SPROP_PROXY_ALWAYS_YES = (1 << 9);
const uint16_t SPROP_CHANGES_OFTEN = (1 << 10);
const uint16_t SPROP_IS_A_VECTOR_ELEM = (1 << 11);
const uint16_t SPROP_COLLAPSIBLE = (1 << 12);
const uint16_t SPROP_COORD_MP = (1 << 13);
const uint16_t SPROP_COORD_MP_LOWPRECISION = (1 << 14);
const uint16_t SPROP_COORD_MP_INTEGRAL = (1 << 15);

// DataTablesMsg
const size_t PROPINFOBITS_NUMPROPS = 10;
const size_t PROPINFOBITS_TYPE = 5;
const size_t PROPINFOBITS_NUMELEMENTS = 10;
const size_t PROPINFOBITS_NUMBITS = 7;
const size_t PROPINFOBITS_FLAGS = 16;

class DemoMessage
{
public:
    DemoMessage(const int32_t& p_tick);
    virtual ~DemoMessage() = default;
    virtual std::string toString() const;

    static const char* name;
    static const MessageType type;
    int32_t tick;
};

class ConsoleCmdMsg : public DemoMessage
{
public:
    ConsoleCmdMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual ~ConsoleCmdMsg() = default;
    virtual std::string toString() const;

    static const char* name;
    static const MessageType type = MessageType::ConsoleCmd;
    std::string command;
};

class UserCmdMsg : public DemoMessage
{
public:
    UserCmdMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual ~UserCmdMsg() = default;
    virtual std::string toString() const;

    static const char* name;
    static const MessageType type = MessageType::UserCmd;
    uint32_t fields;
    uint32_t command_number;
    uint32_t tick_count;
    QAngle viewangles;
    float forwardmove;
    float sidemove;
    float upmove;
    uint32_t buttons;
    uint8_t impulse;
    uint32_t weaponselect;
    uint32_t weaponsubtype;
    int16_t mousedx;
    int16_t mousedy;
};

class SendProp
{
public:
    std::string toString() const;

    SendPropType type;
    std::string name;
    std::string exclude_dt_name;
    uint16_t flags;
    uint16_t num_elements;
    float flow_value;
    float fhigh_value;
    uint8_t nbits;
};

class DataTable
{
public:
    std::string toString() const;

    bool needs_decoder;
    std::string name;
    std::vector<SendProp> props;
};

class DataTablesMsg : public DemoMessage
{
public:
    DataTablesMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual ~DataTablesMsg();
    virtual std::string toString() const;

    static const char* name;
    static const MessageType type = MessageType::DataTables;
    std::vector<DataTable*> tables;
    std::vector<ClassInfo> classes;
};

class SyncTickMsg : public DemoMessage
{
public:
    SyncTickMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual std::string toString() const;

    static const char* name;
    static const MessageType type = MessageType::SyncTick;
};

class Packet : public DemoMessage
{
public:
    Packet(const int32_t& tick, const char* data, const size_t& data_size);
    virtual ~Packet();
    virtual std::string toString() const;

    static const char* name;
    static const MessageType type = MessageType::Packet;
    std::vector<NetMsg*> messages;
};

class Signon : public Packet
{
public:
    Signon(const int32_t& tick, const char* data, const size_t& data_size);
    virtual ~Signon() = default;
    virtual std::string toString() const;

    static const char* name;
    static const MessageType type = MessageType::Signon;
};

class StringTable
{
public:
    StringTable(BitBuffer& buf);
    std::string toString() const;

    std::string name;
    bool has_client_entries;
    std::vector<STableEntry> entries;
    std::vector<STableEntry> entries_client;
};

class StringTablesMsg : public DemoMessage
{
public:
    StringTablesMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual ~StringTablesMsg();
    virtual std::string toString() const;

    static const char* name;
    static const MessageType type = MessageType::StringTables;
    std::vector<StringTable*> tables;
};
