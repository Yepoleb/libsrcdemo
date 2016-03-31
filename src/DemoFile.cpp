#include <string>
#include <stdint.h>
#include <fstream>
#include <cstring>
#include <exception>
#include <iostream>

struct demoheader_t
{
    std::string demofilestamp; // Should be HL2DEMO
    int32_t demoprotocol;     // Should be DEMO_PROTOCOL
    int32_t networkprotocol;  // Should be PROTOCOL_VERSION
    std::string servername;    // Name of server
    std::string clientname;    // Name of client who recorded the game
    std::string mapname;       // Name of map
    std::string gamedirectory; // Name of game directory (com_gamedir)
    float playback_time;       // Time of track
    int32_t playback_ticks;   // # of ticks in track
    int32_t playback_frames;  // # of frames in track
    int32_t signonlength;     // length of sigondata in bytes
};

enum class MessageType
{
    Signon = 1,
    Packet,
    SyncTick,
    ConsoleCmd,
    UserCmd,
    DataTables,
    Stop,
    // CustomData, // L4D2
    StringTables
};

struct DemoMessage
{
    MessageType type;
    int tick;
    std::vector<uint8_t> data;
};

class DemoException : public std::runtime_error
{
    virtual const char* what() const throw()
    {
        return "Failed to parse d            emo."
    }
}

std::vector<char> ReadBytes(std::istream& stream, const size_t& length)
{
    std::vector<char> bytes;
    bytes.reserve(length);
    stream.read(bytes.data(), length);
    return bytes;
}

std::string ReadString(std::istream& stream, const size_t& length)
{
    char* temp = new char[length+1];
    stream.read(temp, length);
    temp[length] = '\0';
    std::string str(temp);
    delete temp;
    return str;
}

int32_t ReadInt8(std::istream& stream)
{
    int8_t value;
    stream.read(&value, sizeof(int8_t));
    return value;
}

int32_t ReadInt32(std::istream& stream)
{
    int32_t value;
    stream.read(reinterpret_cast<char*>(&value), sizeof(int32_t));
    return value;
}

float ReadFloat(std::istream& stream)
{
    float value;
    stream.read(reinterpret_cast<char*>(&value), sizeof(float));
    return value;
}

class DemoFile
{
private:
    std::ifstream m_file;
    DemoInfo m_info;
    std::vector<DemoMessage> m_messages;
}

class DemoFile
{
    public DemoFile(std::ifstream s)
    {
        m_file = s;
        //m_messages = std::vector<DemoMessage>;
        Parse();
    }

    void Parse()
    {
        m_file.seekg(0, std::ios::end);
        m_info.demofilestamp = ReadString(m_file, 8);

        if (demofilestamp != "HL2DEMO") {
            std::cerr << "Unsupported file format." << std::endl;
            throw DemoException;
        }

        m_info.demoprotocol = ReadInt32(m_file);
        if (m_info.demoprotocol >> 8 > 0) {
            std::cerr << "Demos recorded on L4D branch games are currently unsupported." << std::endl;
            throw DemoException;
        }

        m_info.networkprotocol = ReadInt32(m_file);

        m_info.servername = ReadString(m_file, 260);
        m_info.clientname = ReadString(m_file, 260);
        m_info.mapname = ReadString(m_file, 260);
        m_info.gamedirectory = ReadString(m_file, 260);

        m_info.playback_time = ReadFloat(m_file);
        m_info.playback_ticks = ReadInt32(m_file);
        m_info.playback_frames = ReadInt32(m_file);

        m_info.signonlength = ReadInt32(m_file);

        while (true)
        {
            DemoMessage msg;
            msg.type = ReadInt8(m_file);
            if (msg.type == MessageType::Stop)
                break;
            msg.tick = ReadInt32(m_file);

            switch (msg.type)
            {
                case MessageType::Signon:
                case MessageType::Packet:
                case MessageType::ConsoleCmd:
                case MessageType::UserCmd:
                case MessageType::DataTables:
                case MessageType::StringTables:
                    if (msg.type == MessageType::Packet or msg.type == MessageType::Signon)
                        m_file.seekg(0x54, std::ios::cur); // command/sequence info
                    else if (msg.type == MessageType::UserCmd)
                        m_file.seekg(0x4, std::ios::cur); // unknown
                    size_t data_size = ReadInt32(m_file);
                    msg.data = ReadBytes(m_file, data_size);
                    break;
                case MessageType::SyncTick:
                    msg.data = std::vector<char>(); // lol wut
                    break;
                default:
                    std::cerr << "Unknown demo message type encountered." << std::endl;
                    throw DemoException;
            }

            m_messages.push_back(msg);
        }
    }
}

class UserCmd
{
    static void ParseIntoTreeNode(byte[] data, TreeNode node)
    {
        var bb = new BitBuffer(data);
        if (bb.ReadBool()) node.Nodes.Add("Command number: " + bb.ReadBits(32));
        if (bb.ReadBool()) node.Nodes.Add("Tick count: " + bb.ReadBits(32));
        if (bb.ReadBool()) node.Nodes.Add("Viewangle pitch: " + bb.ReadFloat());
        if (bb.ReadBool()) node.Nodes.Add("Viewangle yaw: " + bb.ReadFloat());
        if (bb.ReadBool()) node.Nodes.Add("Viewangle roll: " + bb.ReadFloat());
        if (bb.ReadBool()) node.Nodes.Add("Foward move: " + bb.ReadFloat());
        if (bb.ReadBool()) node.Nodes.Add("Side move: " + bb.ReadFloat());
        if (bb.ReadBool()) node.Nodes.Add("Up move: " + bb.ReadFloat().ToString());
        if (bb.ReadBool()) node.Nodes.Add("Buttons: 0x" + bb.ReadBits(32).ToString("X8"));
        if (bb.ReadBool()) node.Nodes.Add("Impulse: " + bb.ReadBits(8));
        // TODO: weaponselect/weaponsubtype, mousedx/dy
    }
}