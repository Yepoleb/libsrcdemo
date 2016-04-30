#include <stdint.h>
#include <string>
#include <sstream>

#include "synctick.hpp"

SyncTickMsg::SyncTickMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(tick)
{

}

std::string SyncTickMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << getName() << std::endl;
    return ss.str();
}
