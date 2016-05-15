#include <stdint.h>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <iostream>
#include <exception>

#include "bitbuffer.hpp"
#include "helpers.hpp"
#include "stringtable.hpp"

StringTable::StringTable()
{
    has_client_entries = false;
    userdata_fixed_size = false;
    max_entries = 0;
}

void StringTable::readBufferDem(BitBuffer& buf)
{
    clear();
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

std::vector<uint16_t> StringTable::readBufferNet(BitBuffer& buf, const uint16_t num_entries)
{
    size_t encode_bits = std::log2(max_entries);
    std::vector<uint16_t> changed;
    std::vector<std::string> history;
    size_t entry_index = 0;
    for (int32_t i = 0; i < num_entries; i++) {
        if (not buf.readBool()) {
            entry_index = buf.readBits(encode_bits);
        }

        STableEntry entry;
        entry.index = entry_index;
        if (buf.readBool()) {
            bool has_basestring = buf.readBool();
            if (has_basestring) {
                uint8_t substr_index = buf.readBits(5);
                uint8_t substr_length = buf.readBits(SUBSTRING_BITS);
                std::string basestring = history.at(substr_index);
                std::string substring = basestring.substr(0, substr_length);
                entry.name = substring + buf.readString();
            } else {
                entry.name = buf.readString();
            }
        }

        if (buf.readBool()) {
            if (userdata_fixed_size) {
                entry.length = userdata_size_bits;
                entry.data = buf.readData(userdata_size_bits);
            } else {
                size_t entrydata_size = buf.readBits(MAX_USERDATA_BITS) * 8;
                entry.length = entrydata_size;
                entry.data = buf.readData(entrydata_size);
            }
        } else {
            entry.length = 0;
        }

        // Update entry
        if (entry.index < entries.size()) {
            entries.at(entry.index) = entry;
        } else if (entry.index == entries.size()){
            entries.push_back(entry);
        } else {
            std::cerr << "Entry index too large" << std::endl;
            throw std::exception();
        }

        if (history.size() >= 32) {
            history.erase(history.begin());
        }

        history.push_back(entry.name);
        changed.push_back(entry.index);
        entry_index++;
    }

    return changed;
}

void StringTable::clear()
{
    entries.clear();
    entries_client.clear();
}

std::string StringTable::toString() const
{
    std::stringstream ss;
    ss << std::boolalpha;
    ss << "name: " << name << std::endl;
    ss << "max_entries: " << max_entries << std::endl;
    ss << "userdata_fixed_size: " << userdata_fixed_size << std::endl;
    ss << "userdata_size: " << userdata_size << std::endl;
    ss << "userdata_size_bits: " << (int)userdata_size_bits << std::endl;
    ss << "entries:" << std::endl;
    for (const STableEntry& entry : entries) {
        ss << indent(entry.toString(), 2);
    }
    if (has_client_entries) {
        ss << "entries_client:" << std::endl;
        for (const STableEntry& entry : entries_client) {
            ss << indent(entry.toString(), 2);
        }
    }
    return ss.str();
}

std::string StringTable::formatEntries(std::vector<uint16_t> entry_indices) const
{
    std::stringstream ss;
    for (const uint16_t entry_i : entry_indices) {
        ss << entries.at(entry_i).toString();
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
