#include "external/vdfparser.h"
#include <iostream>
#include <cstring>
#include <exception>
#include <string>
#include <vector>
#include <sstream>

#include "EventParser.hpp"
#include "BitBuffer.hpp"

EventParser* g_evtparser;

const char* EVENT_FIELD_TYPE_MAP[] =
{
    "local",       // 0 : don't network this field
    "string",      // 1 : zero terminated ASCII string
    "float",       // 2 : float 32 bit
    "long",        // 3 : signed int 32 bit
    "short",       // 4 : signed int 16 bit
    "byte",        // 5 : unsigned int 8 bit
    "bool",        // 6 : unsigned int 1 bit
};

FieldType parseFieldType(const char* field_str)
{
    for (size_t i_type = 0; i_type < NUM_FIELD_TYPES; i_type++) {
        if (std::strcmp(field_str, EVENT_FIELD_TYPE_MAP[i_type]) == 0) {
            return static_cast<FieldType>(i_type);
        }
    }
    std::cerr << "Unknown field: " << field_str << std::endl;
    throw std::exception();
}

std::string Event::toString() const
{
    std::stringstream ss;
    ss << id << " " << name << std::endl;
    ss << std::boolalpha;
    for (const EventValue& val : values) {
        ss << "  " << EVENT_FIELD_TYPE_MAP[(int)val.type] << " " << val.name << " = ";
        switch (val.type)
        {
            case FieldType::TYPE_LOCAL:
                break;
            case FieldType::TYPE_STRING:
                ss << val.v_string;
                break;
            case FieldType::TYPE_FLOAT:
                ss << val.v_float;
                break;
            case FieldType::TYPE_LONG:
                ss << val.v_long;
                break;
            case FieldType::TYPE_SHORT:
                ss << val.v_short;
                break;
            case FieldType::TYPE_BYTE:
                ss << (int)val.v_byte;
                break;
            case FieldType::TYPE_BOOL:
                ss << val.v_bool;
                break;
            default:
                throw std::exception();
        }
        ss << std::endl;
    }
    return ss.str();
}

void EventParser::loadFile(const std::string& path)
{
    loadFile(path.c_str());
}

void EventParser::loadFile(const char* path)
{
    VDFTree* tree = nullptr;
    if (not VDFTreeParser::OpenVDF(path, &tree)) {
        std::cerr << "Failed to open file " << path << std::endl;
        throw std::exception();
    }

    VDFNode* root = tree->rootNode;

    VDFNode* event_node = root->childNode;
    while (event_node != nullptr) {
        char* event_name = event_node->key;
        EventDescriptor descriptor;
        descriptor.name = event_name;

        // Workaround for parser error
        if (descriptor.name != "{}") {
            VDFNode* field_node = event_node->childNode;
            while (field_node != nullptr) {
                char* field_name = field_node->key;
                if (field_name != nullptr) {
                    FieldType field_type = parseFieldType(field_node->value);
                    EventField field = {field_name, field_type};
                    descriptor.fields.push_back(field);
                }
                field_node = field_node->nextNode;
            }

            size_t old_index = getIndex(event_name);

            if (old_index == SIZE_T_MAX) {
                events.push_back(descriptor);
            } else {
                events[old_index] = descriptor;
            }
        }

        event_node = event_node->nextNode;
    }

    delete tree;
    tree = nullptr;
}

size_t EventParser::getIndex(const std::string& name)
{
    for (size_t i_desc = 0; i_desc < events.size(); i_desc++) {
        EventDescriptor& descriptor = events[i_desc];
        if (descriptor.name == name) {
            return i_desc;
        }
    }
    return SIZE_T_MAX;
}

EventDescriptor* EventParser::getDescriptor(size_t event_id)
{
    if (event_id < events.size()) {
        return &events.at(event_id);
    } else {
        return nullptr;
    }
}

void EventParser::printEvents()
{
    for (size_t i_event = 0; i_event < events.size(); i_event++) {
        const EventDescriptor& descriptor = events[i_event];
        std::cout << "Event " << i_event << " " << descriptor.name << std::endl;
        for (const EventField& field : descriptor.fields) {
            std::cout << "  " << EVENT_FIELD_TYPE_MAP[(int)field.type] << " " << field.name << std::endl;
        }
        std::cout << std::endl;
    }
}

Event EventParser::parseEvent(BitBuffer& buf)
{
    uint16_t event_id = buf.ReadBits(MAX_EVENT_BITS);
    EventDescriptor* desc = getDescriptor(event_id);
    Event event;
    if (desc == nullptr) {
        std::cerr << "Ignored event " << event_id << std::endl;
        event.name = "Unknown";
        event.id = event_id;
        return event;
    }

    event.name = desc->name;
    event.id = event_id;
    for (const EventField& field : desc->fields) {
        EventValue val;
        val.name = field.name;
        val.type = field.type;
        switch (field.type)
        {
            case FieldType::TYPE_LOCAL:
                break;
            case FieldType::TYPE_STRING:
                val.v_string = buf.ReadString();
                break;
            case FieldType::TYPE_FLOAT:
                val.v_float = buf.ReadFloat();
                break;
            case FieldType::TYPE_LONG:
                val.v_long = buf.ReadS32();
                break;
            case FieldType::TYPE_SHORT:
                val.v_short = buf.ReadS16();
                break;
            case FieldType::TYPE_BYTE:
                val.v_byte = buf.ReadU8();
                break;
            case FieldType::TYPE_BOOL:
                val.v_bool = buf.ReadBool();
                break;
            default:
                throw std::exception();
        }

        event.values.push_back(val);
    }

    return event;
}
