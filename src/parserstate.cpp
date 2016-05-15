#include <string>

#include "stringtable.hpp"
#include "parserstate.hpp"

void ParserState::updateStringTable(StringTable* newtable)
{
    // Check for existing table
    for (StringTable*& oldtable : strtables) {
        if (oldtable->name == newtable->name) {
            oldtable = newtable;
            return;
        }
    }
    strtables.push_back(newtable);
}

StringTable* ParserState::getStringTable(const size_t index)
{
    return strtables.at(index);
}

StringTable* ParserState::getStringTable(const std::string name)
{
    for (StringTable* table : strtables) {
        if (table->name == name) {
            return table;
        }
    }
    return nullptr;
}
