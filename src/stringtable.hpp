#pragma once
#include <stdint.h>
#include <string>
#include <vector>

class BitBuffer;
class STableEntry;

const size_t MAX_USERDATA_BITS = 14;
const size_t SUBSTRING_BITS = 5;

class StringTable
{
public:
    StringTable();
    //StringTable(const StringTable& other) = default;
    void readBufferDem(BitBuffer& buf);
    std::vector<uint16_t> readBufferNet(BitBuffer& buf, const uint16_t num_entries);
    void clear();
    std::string toString() const;
    std::string formatEntries(std::vector<uint16_t> entry_indices) const;

    std::string name;
    uint16_t max_entries;
    bool userdata_fixed_size;
    uint16_t userdata_size;
    uint8_t userdata_size_bits;
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
