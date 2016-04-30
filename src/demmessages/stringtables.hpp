#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include "demmessage.hpp"

class STableEntry;
class StringTable;
class BitBuffer;

class StringTablesMsg : public DemoMessage
{
public:
    StringTablesMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual ~StringTablesMsg();
    virtual std::string toString() const;
    virtual std::string getName() const { return "StringTablesMsg"; };
    virtual MessageType getType() const { return MessageType::StringTables; };

    std::vector<StringTable*> tables;
};

class StringTable
{
public:
    StringTable(BitBuffer& buf);
    std::string toString() const;

    std::string name;
    bool has_client_entries;
    std::vector<STableEntry> entries;
    std::vector<STableEntry> entries_client;
};

class STableEntry
{
public:
    STableEntry();
    STableEntry(size_t p_index, const std::string& p_name);
    STableEntry(size_t p_index, const std::string& p_name, size_t p_length, const std::vector<char>& p_data);
    void fromBuffer(size_t p_index, BitBuffer& buf);
    std::string toString() const;

    size_t index;
    std::string name;
    size_t length;
    std::vector<char> data;
};
