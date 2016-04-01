#include <string>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "DemoFile.hpp"

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
        DemoMessage msg;
        msg.type = static_cast<MessageType>(ReadInt8(file));
        if (msg.type == MessageType::Stop)
            break;
        msg.tick = ReadInt32(file);

        switch (msg.type)
        {
            case MessageType::Signon:
            case MessageType::Packet:
            case MessageType::ConsoleCmd:
            case MessageType::UserCmd:
            case MessageType::DataTables:
            case MessageType::StringTables: {
                if (msg.type == MessageType::Packet or msg.type == MessageType::Signon)
                    file.seekg(0x54, std::ios::cur); // command/sequence info
                else if (msg.type == MessageType::UserCmd)
                    file.seekg(0x4, std::ios::cur); // unknown
                size_t data_size = ReadInt32(file);
                msg.data = ReadBytes(file, data_size);
                msg.data_size = data_size;
                //std::cout << "data_size " << data_size << std::endl;
                break;
            }
            case MessageType::SyncTick: {
                msg.data = nullptr; // lol wut
                msg.data_size = 0;
                break;
            }
            default:
                std::cerr << "Unknown demo message type encountered." << std::endl;
                throw DemoException();
        }

        m_messages.push_back(msg);
    }
    file.close();
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
    delete temp;
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
