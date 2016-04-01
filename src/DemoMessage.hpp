#pragma once
#include <stdint.h>
#include <string>

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

struct QAngle
{
    float x;
    float y;
    float z;
};

class DemoMessage
{
public:
    DemoMessage(const MessageType& p_type, const int32_t& p_tick);
    virtual std::string toString() const;

    MessageType type;
    int32_t tick;
};

class ConsoleCmdMsg : public DemoMessage
{
public:
    ConsoleCmdMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual std::string toString() const;

    std::string command;
};

class UserCmdMsg : public DemoMessage
{
public:
    UserCmdMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual std::string toString() const;

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
