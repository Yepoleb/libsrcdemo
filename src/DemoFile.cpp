#include <string>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "DemoFile.hpp"
#include "DemoMessage.hpp"

DemoFile::DemoFile(std::string filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    m_header.demofilestamp = ReadString(file, 8);

    if (m_header.demofilestamp != "HL2DEMO") {
        std::cerr << "Unsupported file format." << std::endl;
        throw DemoException();
    }

    m_header.demoprotocol = ReadInt32(file);
    if (m_header.demoprotocol >> 8 > 0) {
        std::cerr << "Demos recorded on L4D branch games are currently unsupported." << std::endl;
        throw DemoException();
    }

    m_header.networkprotocol = ReadInt32(file);

    m_header.servername = ReadString(file, 260);
    m_header.clientname = ReadString(file, 260);
    m_header.mapname = ReadString(file, 260);
    m_header.gamedirectory = ReadString(file, 260);

    m_header.playback_time = ReadFloat(file);
    m_header.playback_ticks = ReadInt32(file);
    m_header.playback_frames = ReadInt32(file);

    m_header.signonlength = ReadInt32(file);

    while (true)
    {
        DemoMessage* msg;
        MessageType type = static_cast<MessageType>(ReadInt8(file));
        if (type == MessageType::Stop)
            break;
        int32_t tick = ReadInt32(file);
        char* data;
        size_t data_size;

        switch (type)
        {
            case MessageType::Signon:
            case MessageType::Packet:
            case MessageType::ConsoleCmd:
            case MessageType::UserCmd:
            case MessageType::DataTables:
            case MessageType::StringTables: {
                if (type == MessageType::Packet or type == MessageType::Signon)
                    file.seekg(0x54, std::ios::cur); // command/sequence info
                else if (type == MessageType::UserCmd)
                    file.seekg(0x4, std::ios::cur); // unknown
                data_size = ReadInt32(file);
                data = ReadBytes(file, data_size);
                break;
            }
            case MessageType::SyncTick: {
                data = nullptr; // lol wut
                data_size = 0;
                break;
            }
            default:
                std::cerr << "Unknown demo message type encountered." << std::endl;
                throw DemoException();
        }

        switch (type)
        {
            case MessageType::Signon:
                msg = static_cast<DemoMessage*>(new Signon(tick, data, data_size));
                break;
            case MessageType::Packet:
                msg = static_cast<DemoMessage*>(new Packet(tick, data, data_size));
                break;
            case MessageType::ConsoleCmd:
                msg = static_cast<DemoMessage*>(new ConsoleCmdMsg(tick, data, data_size));
                break;
            case MessageType::UserCmd:
                msg = static_cast<DemoMessage*>(new UserCmdMsg(tick, data, data_size));
                break;
            case MessageType::DataTables:
                msg = static_cast<DemoMessage*>(new DataTablesMsg(tick, data, data_size));
                break;
            case MessageType::StringTables:
                msg = static_cast<DemoMessage*>(new StringTablesMsg(tick, data, data_size));
                break;
            case MessageType::SyncTick:
                msg = static_cast<DemoMessage*>(new SyncTickMsg(tick, data, data_size));
                break;
            default:
                std::cerr << "Unknown demo message type encountered." << std::endl;
                throw DemoException();
        }

        if (data != nullptr) {
            delete[] data;
        }
        if (msg != nullptr)
            m_messages.push_back(msg);
    }
    file.close();
}

DemoFile::~DemoFile()
{
    for (DemoMessage* msg : m_messages) {
        delete msg;
    }
}



char* ReadBytes(std::istream& stream, const size_t& length)
{
    char* bytes = new char[length];
    stream.read(bytes, length);
    return bytes;
}

std::string ReadString(std::istream& stream, const size_t& length)
{
    char* temp = new char[length+1];
    stream.read(temp, length);
    temp[length] = '\0';
    std::string str(temp);
    delete[] temp;
    return str;
}

int8_t ReadInt8(std::istream& stream)
{
    int8_t value;
    stream.read(reinterpret_cast<char*>(&value), sizeof(int8_t));
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
