#include <stdint.h>
#include <string>
#include <sstream>

#include "demmessage.hpp"

DemoMessage::DemoMessage(const int32_t& p_tick)
{
    tick = p_tick;
}

std::string DemoMessage::toString() const
{
    std::stringstream ss;
    ss << tick << " " << getName() << std::endl;
    ss << "  Unknown Data" << std::endl;
    return ss.str();
}

