#include <stdint.h>
#include <string>
#include <sstream>

#include "bitbuffer.hpp"
#include "helpers.hpp"
#include "common.hpp"
#include "config.hpp"
#include "stringtables.hpp"

StringTablesMsg::StringTablesMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    DemoMessage(tick)
{
#ifdef ENABLE_STRINGTABLESMSG
    BitBuffer buf(data, data_size * 8);
    uint8_t num_tables = buf.readU8();
    for (size_t i_table = 0; i_table < num_tables; i_table++) {
        StringTable* table = new StringTable(buf);
        tables.push_back(table);
    }
#endif
}

StringTablesMsg::~StringTablesMsg()
{
    for (StringTable* table : tables) {
        delete table;
    }
}

std::string StringTablesMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << getName() << std::endl;
#ifdef ENABLE_STRINGTABLESMSG
    for (StringTable* table : tables) {
        ss << indent(table->toString(), 2);
    }
#else
    ss << "  Skipped StringTableMsg parsing" << std::endl;
#endif
    return ss.str();
}

StringTable::StringTable(BitBuffer& buf)
{
    name = buf.readString();
    uint16_t num_entries = buf.readU16();
    for (size_t i_entry = 0; i_entry < num_entries; i_entry++) {
        STableEntry entry;
        entry.fromBuffer(i_entry, buf);
        entries.push_back(entry);
    }
    has_client_entries = buf.readBool();
    if (has_client_entries) {
        uint16_t num_entries_client = buf.readU16();
        for (size_t i_entry = 0; i_entry < num_entries_client; i_entry++) {
            STableEntry entry;
            entry.fromBuffer(i_entry, buf);
            entries_client.push_back(entry);
        }
    }
}

std::string StringTable::toString() const
{
    std::stringstream ss;
    ss << name << std::endl;
    ss << "  entries:" << std::endl;
    for (const STableEntry& entry : entries) {
        ss << indent(entry.toString(), 2);
    }
    if (has_client_entries) {
        ss << "  entries_client:" << std::endl;
        for (const STableEntry& entry : entries) {
            ss << indent(entry.toString(), 2);
        }
    }
    return ss.str();
}

STableEntry::STableEntry()
{
    index = 0;
    length = 0;
}

STableEntry::STableEntry(size_t p_index, const std::string& p_name)
{
    index = p_index;
    name = p_name;
    length = 0;
}

STableEntry::STableEntry(size_t p_index, const std::string& p_name, size_t p_length, const std::vector<char>& p_data)
{
    index = p_index;
    name = p_name;
    length = p_length;
    data = p_data;
}

void STableEntry::fromBuffer(size_t p_index, BitBuffer& buf)
{
    index = p_index;
    name = buf.readString();
    bool has_data = buf.readBool();
    if (has_data) {
        // Length in bytes!
        length = buf.readU16() * 8;
        data = buf.readData(length);
    } else {
        length = 0;
    }
}

std::string STableEntry::toString() const
{
    std::stringstream ss;
    ss << index << ": " << name << std::endl;
    if (length) {
        ss << format_data(data) << std::endl;
    }
    return ss.str();
}
