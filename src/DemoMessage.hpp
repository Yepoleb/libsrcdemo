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

enum UserCmdFields
{
    has_command_number = (1 << 0),
    has_tick_count = (1 << 1),
    has_viewangles_x = (1 << 2),
    has_viewangles_y = (1 << 3),
    has_viewangles_z = (1 << 4),
    has_forwardmove = (1 << 5),
    has_sidemove = (1 << 6),
    has_upmove = (1 << 7),
    has_buttons = (1 << 8),
    has_impulse = (1 << 9),
    has_weaponselect = (1 << 10),
    has_weaponsubtype = (1 << 11),
    has_mousedx = (1 << 12),
    has_mousedy = (1 << 13),
};

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

class DemoMessage
{
public:
    DemoMessage(const MessageType& p_type, const int32_t& p_tick);
    virtual std::string toString() const;

    static const char* name;
    MessageType type;
    int32_t tick;
};

class ConsoleCmdMsg : public DemoMessage
{
public:
    ConsoleCmdMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual std::string toString() const;

    static const char* name;
    std::string command;
};

class UserCmdMsg : public DemoMessage
{
public:
    UserCmdMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual std::string toString() const;

    static const char* name;
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
    ~DataTablesMsg();
    virtual std::string toString() const;

    static const char* name;
    std::vector<DataTable*> tables;
    std::vector<ClassInfo> classes;
};

class SyncTickMsg : public DemoMessage
{
public:
    SyncTickMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual std::string toString() const;

    static const char* name;
};

class Packet : public DemoMessage
{
public:
    Packet(const int32_t& tick, const char* data, const size_t& data_size);
    ~Packet();
    virtual std::string toString() const;

    static const char* name;
    std::vector<NetMsg*> messages;
};

class Signon : public Packet
{
public:
    Signon(const int32_t& tick, const char* data, const size_t& data_size);
    virtual std::string toString() const;

    static const char* name;
};

class STableEntry
{
public:
    STableEntry(BitBuffer& buf);
    std::string toString() const;

    std::string name;
    size_t length;
    std::vector<char> data;
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
    ~StringTablesMsg();
    virtual std::string toString() const;

    static const char* name;
    std::vector<StringTable*> tables;
};
