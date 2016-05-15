#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>

#include "bitbuffer.hpp"
#include "helpers.hpp"
#include "common.hpp"
#include "config.hpp"
#include "stringtable.hpp"
#include "stringtables.hpp"

StringTablesMsg::StringTablesMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st) :
    DemoMessage(tick)
{
    readBuffer(data, data_size, parser_st);
}

void StringTablesMsg::readBuffer(const char* data, const size_t data_size, ParserState* parser_st)
{
    BitBuffer buf(data, data_size * 8);
    uint8_t num_tables = buf.readU8();
    for (size_t i_table = 0; i_table < num_tables; i_table++) {
        std::string name = buf.readString();
        StringTable* table_ptr = parser_st->getStringTable(name);
        if (table_ptr == nullptr) {
            std::cerr << "Unknown stringtable: " << name << std::endl;
            continue;
        }
        StringTable table = StringTable(*table_ptr);
        try {
            table.readBufferDem(buf);
        } catch (const std::out_of_range& e) {
            std::cerr << "Malformed stringtable: " << name << std::endl;
            break;
        }
        tables.push_back(table);
        parser_st->updateStringTable(&tables.back());
    }
}

std::string StringTablesMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << getName() << std::endl;
    for (const StringTable& table : tables) {
        ss << indent(table.toString(), 2);
    }
    return ss.str();
}
