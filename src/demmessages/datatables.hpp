#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include "demmessage.hpp"

struct DataTable;
struct SendProp;

extern const char* SENDPROPTYPE_NAMES[];

// SendProp
const uint16_t SPROP_UNSIGNED = (1 << 0);
const uint16_t SPROP_COORD = (1 << 1);
const uint16_t SPROP_NOSCALE = (1 << 2);
const uint16_t SPROP_ROUNDDOWN = (1 << 3);
const uint16_t SPROP_ROUNDUP = (1 << 4);
const uint16_t SPROP_NORMAL = (1 << 5);
const uint16_t SPROP_EXCLUDE = (1 << 6);
const uint16_t SPROP_XYZE = (1 << 7);
const uint16_t SPROP_INSIDEARRAY = (1 << 8);
const uint16_t SPROP_PROXY_ALWAYS_YES = (1 << 9);
const uint16_t SPROP_CHANGES_OFTEN = (1 << 10);
const uint16_t SPROP_IS_A_VECTOR_ELEM = (1 << 11);
const uint16_t SPROP_COLLAPSIBLE = (1 << 12);
const uint16_t SPROP_COORD_MP = (1 << 13);
const uint16_t SPROP_COORD_MP_LOWPRECISION = (1 << 14);
const uint16_t SPROP_COORD_MP_INTEGRAL = (1 << 15);

// DataTablesMsg
const size_t PROPINFOBITS_NUMPROPS = 10;
const size_t PROPINFOBITS_TYPE = 5;
const size_t PROPINFOBITS_NUMELEMENTS = 10;
const size_t PROPINFOBITS_NUMBITS = 7;
const size_t PROPINFOBITS_FLAGS = 16;

enum class SendPropType
{
    DPT_Int = 0,
    DPT_Float,
    DPT_Vector,
    DPT_VectorXY,
    DPT_String,
    DPT_Array,
    DPT_DataTable,
    DPT_Int64,
    DPT_NUMPropTypes
};

class DataTablesMsg : public DemoMessage
{
public:
    DataTablesMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual ~DataTablesMsg();
    virtual std::string toString() const;
    virtual std::string getName() const { return "DataTablesMsg"; }
    virtual MessageType getType() const { return MessageType::DATATABLES; }

    std::vector<DataTable*> tables;
    std::vector<ClassInfo> classes;
};

struct DataTable
{
    std::string toString() const;

    bool needs_decoder;
    std::string name;
    std::vector<SendProp> props;
};

struct SendProp
{
    std::string toString() const;

    SendPropType type;
    std::string name;
    std::string exclude_dt_name;
    uint16_t flags;
    uint16_t num_elements;
    float flow_value;
    float fhigh_value;
    uint8_t nbits;
};
