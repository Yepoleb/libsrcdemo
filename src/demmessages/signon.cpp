#include <stdint.h>
#include <string>

#include "packet.hpp"
#include "common.hpp"
#include "signon.hpp"

SignonMsg::SignonMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st) :
    PacketMsg(tick, data, data_size, parser_st)
{

}
