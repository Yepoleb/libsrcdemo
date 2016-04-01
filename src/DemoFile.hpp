#include <string>
#include <stdint.h>
#include <vector>
#include <exception>

enum class MessageType
{
    Signon = 1,
    Packet,
    SyncTick,
    ConsoleCmd,
    UserCmd,
    DataTables,
    Stop,
    StringTables
};

struct DemoHeader
{
    std::string demofilestamp;
    int32_t demoprotocol;
    int32_t networkprotocol;
    std::string servername;
    std::string clientname;
    std::string mapname;
    std::string gamedirectory;
    float playback_time;
    int32_t playback_ticks;
    int32_t playback_frames;
    int32_t signonlength;
};

struct DemoMessageBase
{
    MessageType type;
    int tick;
};

struct DemoMessage : DemoMessageBase
{
    char* data;
    size_t data_size;
};

struct ConsoleCmdMsg : DemoMessageBase
{
    std::string command;
};

struct QAngle
{
    float x;
    float y;
    float z;
}

struct UserCmdMsg : DemoMessageBase
{
    int32_t command_number;
    int32_t tick_count;
    QAngle viewangles;
    float forwardmove;
    float sidemove;
    float upmove;
    int32_t buttons;
    int8_t impulse;
    int32_t weaponselect;
    int32_t weaponsubtype;
    int32_t random_seed;
    int16_t mousedx;
    int16_t mousedy;
    bool hasbeenpredicted;
}

class DemoException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Failed to parse demo.";
    }
};

class DemoFile
{
public:
    DemoFile(std::string filename);
    DemoHeader getHeader() { return m_header; };
    std::vector<DemoMessage> getMessages() { return m_messages; };
private:
    DemoHeader m_header;
    std::vector<DemoMessage> m_messages;
};

char* ReadBytes(std::istream& stream, const size_t& length);
std::string ReadString(std::istream& stream, const size_t& length);
int8_t ReadInt8(std::istream& stream);
int32_t ReadInt32(std::istream& stream);
float ReadFloat(std::istream& stream);
