#pragma once
#include <string>
#include <vector>

#include "BitBuffer.hpp"

const size_t NUM_FIELD_TYPES = 7;

class EventParser;
extern EventParser* g_evtparser;

enum class FieldType
{
    TYPE_LOCAL = 0,   // not networked
    TYPE_STRING,      // zero terminated ASCII string
    TYPE_FLOAT,       // float 32 bit
    TYPE_LONG,        // signed int 32 bit
    TYPE_SHORT,       // signed int 16 bit
    TYPE_BYTE,        // unsigned int 8 bit
    TYPE_BOOL         // unsigned int 1 bit
};

struct EventField
{
    std::string name;
    FieldType type;
};

struct EventDescriptor
{
    std::string name;
    std::vector<EventField> fields;
};

struct EventValue
{
    std::string name;
    FieldType type;
    std::string v_string;
    union
    {
        float v_float;
        int32_t v_long;
        int16_t v_short;
        uint8_t v_byte;
        bool v_bool;
    };
};

struct Event
{
    std::string toString() const;

    std::string name;
    uint16_t id;
    std::vector<EventValue> values;
};

class EventParser
{
public:
    EventParser() { };
    void loadFile(const char* path);
    void loadFile(const std::string& path);
    size_t getIndex(const std::string& name);
    EventDescriptor* getDescriptor(size_t event_id);
    void printEvents();
    Event parseEvent(BitBuffer& buf);
public:
    std::vector<EventDescriptor> events;
};
