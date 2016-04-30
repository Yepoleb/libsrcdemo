#include <stdint.h>
#include <string>
#include <sstream>

#include "consolecmd.hpp"

ConsoleCmdMsg::ConsoleCmdMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(tick)
{
    // Subtract 1 from data_size to remove the trailing zero bit
    command = std::string(data, data_size - 1);
}

std::string ConsoleCmdMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << getName() << std::endl;
    ss << "  Command: " << command << std::endl;
    return ss.str();
}
