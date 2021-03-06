#include <stdint.h>
#include <string>
#include <sstream>

#include "bitbuffer.hpp"
#include "helpers.hpp"
#include "common.hpp"
#include "datatables.hpp"

const char* SENDPROPTYPE_NAMES[] =
{
    "Int",
    "Float",
    "Vector",
    "VectorXY",
    "String",
    "Array",
    "DataTable",
    "Int64"
};

DataTablesMsg::DataTablesMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st) :
    DemoMessage(tick)
{
    readBuffer(data, data_size, parser_st);
}

void DataTablesMsg::readBuffer(const char* data, const size_t data_size, ParserState* parser_st)
{
    BitBuffer buf(data, data_size * 8);

    size_t nflag_bits = PROPINFOBITS_FLAGS;
    if (DEMOPROTOCOL_VERSION == 2) {
        nflag_bits = 11;
    }

    while (buf.readBool()) {
        DataTable* table = new DataTable;
        table->needs_decoder = buf.readBool();
        table->name = buf.readString();
        uint16_t num_props = buf.readBits(PROPINFOBITS_NUMPROPS);
        for (size_t i_prop = 0; i_prop < num_props; i_prop++) {
            SendProp prop;
            prop.type = static_cast<SendPropType>(buf.readBits(PROPINFOBITS_TYPE));
            prop.name = buf.readString();
            prop.flags = buf.readBits(nflag_bits);

            bool is_exclude = prop.flags & SPROP_EXCLUDE;
            if (prop.type == SendPropType::DPT_DataTable or is_exclude) {
                prop.exclude_dt_name = buf.readString();
            } else if (prop.type == SendPropType::DPT_Array) {
                prop.num_elements = buf.readBits(PROPINFOBITS_NUMELEMENTS);
            } else {
                prop.flow_value = buf.readFloat();
                prop.fhigh_value = buf.readFloat();
                prop.nbits = buf.readBits(PROPINFOBITS_NUMBITS);
            }
            table->props.push_back(prop);
        }
        tables.push_back(table);
    }

    uint16_t num_classes = buf.readU16();
    for (size_t i_class = 0; i_class < num_classes; i_class++) {
        ClassInfo info;
        info.class_id = buf.readU16();
        info.classname = buf.readString();
        info.tablename = buf.readString();
        classes.push_back(info);
    }
}

DataTablesMsg::~DataTablesMsg()
{
    for (DataTable* table : tables) {
        delete table;
    }
}

std::string DataTablesMsg::toString() const
{
    std::stringstream ss;
    ss << tick << " " << getName() << std::endl;
    ss << "  tables:" << std::endl;
    for (DataTable* table : tables) {
        ss << indent(table->toString(), 4) << std::endl;
    }
    ss << "  classes:" << std::endl;
    for (const ClassInfo& class_info : classes) {
        ss << "    class_id: " << class_info.class_id << std::endl;
        ss << "    classname: " << class_info.classname << std::endl;
        ss << "    tablename: " << class_info.tablename << std::endl;
        ss << std::endl;
    }
    return ss.str();
}

std::string DataTable::toString() const
{
    std::stringstream ss;
    ss << std::boolalpha;
    ss << name << " (decoder: " << needs_decoder << ")" << std::endl;
    for (const SendProp& prop : props) {
        ss << indent(prop.toString(), 2);
    }
    return ss.str();
}

std::string SendProp::toString() const
{
    std::stringstream ss;
    uint16_t temp_flags = flags;
    switch (type) {
        case SendPropType::DPT_DataTable:
            ss << exclude_dt_name << " " << name;
            break;
        case SendPropType::DPT_Int:
        case SendPropType::DPT_Int64:
            if (temp_flags & SPROP_UNSIGNED) {
                if (nbits != 1) {
                    ss << "U";
                }
                // Remove unsigned flag
                temp_flags &= ~SPROP_UNSIGNED;
            }
            if (nbits == 1) {
                ss << "Bool";
            } else {
                ss << SENDPROPTYPE_NAMES[(int)type];
                if (nbits > 0) {
                    ss << (int)nbits;
                }
            }
            ss << " " << name;
            break;
        case SendPropType::DPT_Float:
        case SendPropType::DPT_Vector:
        case SendPropType::DPT_VectorXY:
            ss << SENDPROPTYPE_NAMES[(int)type];
            if (nbits > 0) {
                ss << (int)nbits;
            }
            ss << " " << name;
            if (flow_value or fhigh_value) {
                ss << ", range: " << flow_value << " - " << fhigh_value;
            }
            break;
        case SendPropType::DPT_String:
            ss << SENDPROPTYPE_NAMES[(int)type] << " ";
            ss << name;
            break;
        case SendPropType::DPT_Array:
            ss << SENDPROPTYPE_NAMES[(int)type];
            ss << "[" << num_elements << "] " << name;
            break;
        default:
            ss << "Unknown" << (int)type << " " << name;
            break;
    }

    if (temp_flags) {
        ss << ", flags: 0x" << std::hex << temp_flags << std::dec;
    }
    ss << std::endl;

    return ss.str();
}
