#pragma once
#include <stdint.h>
#include <vector>
#include <string>

class StringTable;

class ParserState
{
public:
    void updateStringTable(StringTable* table);
    StringTable* getStringTable(const size_t index);
    StringTable* getStringTable(const std::string name);

    int32_t tick;
    std::vector<StringTable*> strtables;
};
