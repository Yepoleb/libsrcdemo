#pragma once
#include <string>
#include <vector>

class EventParser;
class BitBuffer;

extern EventParser* g_evtparser;

const size_t MAX_EVENT_BITS = 9;
const size_t NUM_FIELD_TYPES = 7;

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

FieldType parseFieldType(const char* field_str);

struct EventField
{
    FieldType type;
    std::string name;
};

struct EventDescriptor
{
    std::string toString() const;

    uint16_t id;
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
    bool addDescriptor(EventDescriptor desc);
    EventDescriptor* getDescriptor(const size_t event_id);
    EventDescriptor* getDescriptor(const std::string& name);
    void printEvents();
    Event parseEvent(BitBuffer& buf);
private:
    std::vector<EventDescriptor> descriptors;
};
