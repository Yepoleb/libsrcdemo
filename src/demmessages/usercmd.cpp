#include <stdint.h>
#include <string>
#include <sstream>

#include "demmessage.hpp"
#include "bitbuffer.hpp"
#include "usercmd.hpp"

UserCmdMsg::UserCmdMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(tick)
{
    BitBuffer buf(data, data_size * 8);
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
