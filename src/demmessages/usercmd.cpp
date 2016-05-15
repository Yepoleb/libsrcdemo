#include <stdint.h>
#include <string>
#include <sstream>

#include "demmessage.hpp"
#include "bitbuffer.hpp"
#include "common.hpp"
#include "usercmd.hpp"

UserCmdMsg::UserCmdMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st) :
    DemoMessage(tick)
{
    readBuffer(data, data_size, parser_st);
}

void UserCmdMsg::readBuffer(const char* data, const size_t data_size, ParserState* parser_st)
{
    BitBuffer buf(data, data_size * 8);
    fields = 0;

    if (buf.readBool()) {
        command_number = buf.readU32();
        fields |= USERCMD_COMMAND_NUMBER;
    }
    if (buf.readBool()) {
        tick_count = buf.readU32();
        fields |= USERCMD_TICK_COUNT;
    }
    if (buf.readBool()) {
        viewangles.x = buf.readFloat();
        fields |= USERCMD_VIEWANGLES_X;
    }
    if (buf.readBool()) {
        viewangles.y = buf.readFloat();
        fields |= USERCMD_VIEWANGLES_Y;
    }
    if (buf.readBool()) {
        viewangles.z = buf.readFloat();
        fields |= USERCMD_VIEWANGLES_Z;
    }
    if (buf.readBool()) {
        forwardmove = buf.readFloat();
        fields |= USERCMD_FORWARDMOVE;
    }
    if (buf.readBool()) {
        sidemove = buf.readFloat();
        fields |= USERCMD_SIDEMOVE;
    }
    if (buf.readBool()) {
        upmove = buf.readFloat();
        fields |= USERCMD_UPMOVE;
    }
    if (buf.readBool()) {
        buttons = buf.readU32();
        fields |= USERCMD_BUTTONS;
    }
    if (buf.readBool()) {
        impulse = buf.readU8();
        fields |= USERCMD_IMPULSE;
    }
    if (buf.readBool()) {
        weaponselect = buf.readBits(MAX_EDICT_BITS);
        fields |= USERCMD_WEAPONSELECT;
        if (buf.readBool()) {
            weaponsubtype = buf.readBits(WEAPON_SUBTYPE_BITS);
            fields |= USERCMD_WEAPONSUBTYPE;
        }
    }
    if (buf.readBool()) {
        mousedx = buf.readS16();
        fields |= USERCMD_MOUSEDX;
    }
    if (buf.readBool()) {
        mousedy = buf.readS16();
        fields |= USERCMD_MOUSEDY;
    }
}

std::string UserCmdMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << getName() << std::endl;
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
