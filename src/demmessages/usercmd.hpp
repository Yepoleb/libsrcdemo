#pragma once
#include <stdint.h>
#include <string>

#include "common.hpp"
#include "demmessage.hpp"

const size_t WEAPON_SUBTYPE_BITS = 6;

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

class UserCmdMsg : public DemoMessage
{
public:
    UserCmdMsg() : DemoMessage() { }
    UserCmdMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st);
    virtual ~UserCmdMsg() = default;
    virtual void readBuffer(const char* data, const size_t data_size, ParserState* parser_st);
    virtual std::string toString() const;
    virtual std::string getName() const { return "UserCmdMsg"; }
    virtual MessageType getType() const { return MessageType::USERCMD; }

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
