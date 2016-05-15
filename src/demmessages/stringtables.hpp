#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include "demmessage.hpp"
#include "stringtable.hpp"
#include "common.hpp"

class STableEntry;
class StringTable;
class BitBuffer;

class StringTablesMsg : public DemoMessage
{
public:
    StringTablesMsg() : DemoMessage() { }
    StringTablesMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st);
    virtual void readBuffer(const char* data, const size_t data_size, ParserState* parser_st);
    virtual std::string toString() const;
    virtual std::string getName() const { return "StringTablesMsg"; }
    virtual MessageType getType() const { return MessageType::STRINGTABLES; }

    std::vector<StringTable> tables;
};
