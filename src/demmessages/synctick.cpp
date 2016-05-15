#include <stdint.h>
#include <string>
#include <sstream>

#include "common.hpp"
#include "synctick.hpp"

SyncTickMsg::SyncTickMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st) :
    DemoMessage(tick)
{
    readBuffer(data, data_size, parser_st);
}

void SyncTickMsg::readBuffer(const char* data, const size_t data_size, ParserState* parser_st)
{

}

std::string SyncTickMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << getName() << std::endl;
    return ss.str();
}
