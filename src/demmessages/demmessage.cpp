#include <stdint.h>
#include <string>
#include <sstream>

#include "demmessage.hpp"

DemoMessage::DemoMessage()
{
    tick = 0;
}

DemoMessage::DemoMessage(int32_t p_tick)
{
    tick = p_tick;
}
