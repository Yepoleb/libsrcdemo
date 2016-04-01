#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>

#include "DemoMessage.hpp"
#include "BitBuffer.hpp"

const size_t MAX_EDICT_BITS = 11;
const size_t WEAPON_SUBTYPE_BITS = 6;

DemoMessage::DemoMessage(const MessageType& p_type, const int32_t& p_tick)
{
    type = p_type;
    tick = p_tick;
}

std::string DemoMessage::toString() const
{
    std::stringstream ss;
    ss << tick << " DemoMessage " << std::endl;
    ss << "  Unknown Data" << std::endl;
    return ss.str();
}

ConsoleCmdMsg::ConsoleCmdMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(MessageType::ConsoleCmd, tick)
{
    command = std::string(data, data_size);
}

std::string ConsoleCmdMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " ConsoleCmd" << std::endl;
    ss << "  Command: " << command << std::endl;
    return ss.str();
}

UserCmdMsg::UserCmdMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(MessageType::UserCmd, tick)
{
//    command_number = 0;
//    tick_count = 0;
//    viewangles = {0.0f, 0.0f, 0.0f};
//    forwardmove = 0.0f;
//    sidemove = 0.0f;
//    upmove = 0.0f;
//    buttons = 0;
//    impulse = 0;
//    weaponselect = 0;
//    weaponsubtype = 0;
//    mousedx = 0;
//    mousedy = 0;
    fields = 0;
    
    BitBuffer buf(data, data_size);
    if (buf.ReadBool()) {
        command_number = buf.ReadU32();
        fields |= has_command_number;
    }
    if (buf.ReadBool()) {
        tick_count = buf.ReadU32();
        fields |= has_tick_count;
    }
    if (buf.ReadBool()) {
        viewangles.x = buf.ReadFloat();
        fields |= has_viewangles_x;
    }
    if (buf.ReadBool()) {
        viewangles.y = buf.ReadFloat();
        fields |= has_viewangles_y;
    }
    if (buf.ReadBool()) {
        viewangles.z = buf.ReadFloat();
        fields |= has_viewangles_z;
    }
    if (buf.ReadBool()) {
        forwardmove = buf.ReadFloat();
        fields |= has_forwardmove;
    }
    if (buf.ReadBool()) {
        sidemove = buf.ReadFloat();
        fields |= has_sidemove;
    }
    if (buf.ReadBool()) {
        upmove = buf.ReadFloat();
        fields |= has_upmove;
    }
    if (buf.ReadBool()) {
        buttons = buf.ReadU32();
        fields |= has_buttons;
    }
    if (buf.ReadBool()) {
        impulse = buf.ReadU8();
        fields |= has_impulse;
    }
    if (buf.ReadBool()) {
        weaponselect = buf.ReadBits(MAX_EDICT_BITS);
        fields |= has_weaponselect;
        if (buf.ReadBool()) {
            weaponsubtype = buf.ReadBits(WEAPON_SUBTYPE_BITS);
            fields |= has_weaponsubtype;
        }
    }
    if (buf.ReadBool()) {
        mousedx = buf.ReadS16();
        fields |= has_mousedx;
    }
    if (buf.ReadBool()) {
        mousedy = buf.ReadS16();
        fields |= has_mousedy;
    }
}

std::string UserCmdMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " UserCmdMsg" << std::endl;
    if (fields & has_command_number)
        ss << "Command number: " << command_number << std::endl;
    if (fields & has_tick_count)
        ss << "Tick count: " << tick_count << std::endl;
    if (fields & has_viewangles_x)
        ss << "Viewangle pitch: " << viewangles.x << std::endl;
    if (fields & has_viewangles_y)
        ss << "Viewangle yaw: " << viewangles.y << std::endl;
    if (fields & has_viewangles_z)
        ss << "Viewangle roll: " << viewangles.z << std::endl;
    if (fields & has_forwardmove)
        ss << "Foward move: " << forwardmove << std::endl;
    if (fields & has_sidemove)
        ss << "Side move: " << sidemove << std::endl;
    if (fields & has_upmove)
        ss << "Up move: " << upmove << std::endl;
    if (fields & has_buttons)
        ss << "Buttons: 0x" << std::hex << buttons << std::dec << std::endl;
    if (fields & has_impulse)
        ss << "Impulse: " << impulse << std::endl;
    if (fields & has_weaponselect)
        ss << "Weaponselect: " << weaponselect << std::endl;
    if (fields & has_weaponsubtype)
        ss << "Weaponsubtype: " << weaponsubtype << std::endl;
    if (fields & has_mousedx)
        ss << "Mouse DX: " << mousedx << std::endl;
    if (fields & has_mousedy)
        ss << "Mouse DY: " << mousedy << std::endl;
    return ss.str();
}
