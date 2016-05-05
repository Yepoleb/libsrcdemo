#include <string>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "demmessages/demmessage.hpp"
#include "demmessages/signon.hpp"
#include "demmessages/packet.hpp"
#include "demmessages/synctick.hpp"
#include "demmessages/consolecmd.hpp"
#include "demmessages/usercmd.hpp"
#include "demmessages/datatables.hpp"
#include "demmessages/stringtables.hpp"
#include "demofile.hpp"

DemoFile::DemoFile(const std::string& filename)
{
    load(filename);
}

void DemoFile::load(const std::string& filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (not file.is_open()) {
        std::cerr << "Could not open demo file" << std::endl;
        throw DemoException();
    }
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
        if (type == MessageType::STOP)
            break;
        int32_t tick = ReadInt32(file);
        char* data;
        size_t data_size;

        switch (type)
        {
            case MessageType::SIGNON:
            case MessageType::PACKET:
            case MessageType::CONSOLECMD:
            case MessageType::USERCMD:
            case MessageType::DATATABLES:
            case MessageType::STRINGTABLES: {
                if (type == MessageType::PACKET or type == MessageType::SIGNON)
                    file.seekg(0x54, std::ios::cur); // command/sequence info
                else if (type == MessageType::USERCMD)
                    file.seekg(0x4, std::ios::cur); // unknown
                data_size = ReadInt32(file);
                data = ReadBytes(file, data_size);
                break;
            }
            case MessageType::SYNCTICK: {
                data = nullptr;
                data_size = 0;
                break;
            }
            default:
                std::cerr << "Unknown demo message type encountered." << std::endl;
                throw DemoException();
        }

        switch (type)
        {
            case MessageType::SIGNON:
                msg = static_cast<DemoMessage*>(new SignonMsg(tick, data, data_size));
                break;
            case MessageType::PACKET:
                msg = static_cast<DemoMessage*>(new PacketMsg(tick, data, data_size));
                break;
            case MessageType::CONSOLECMD:
                msg = static_cast<DemoMessage*>(new ConsoleCmdMsg(tick, data, data_size));
                break;
            case MessageType::USERCMD:
                msg = static_cast<DemoMessage*>(new UserCmdMsg(tick, data, data_size));
                break;
            case MessageType::DATATABLES:
                msg = static_cast<DemoMessage*>(new DataTablesMsg(tick, data, data_size));
                break;
            case MessageType::STRINGTABLES:
                msg = static_cast<DemoMessage*>(new StringTablesMsg(tick, data, data_size));
                break;
            case MessageType::SYNCTICK:
                msg = static_cast<DemoMessage*>(new SyncTickMsg(tick, data, data_size));
                break;
            default:
                std::cerr << "Unknown demo message type encountered." << std::endl;
                throw DemoException();
        }

        if (data != nullptr) {
            delete[] data;
        }
        if (msg != nullptr) {
            //std::cout << msg->toString();
            m_messages.push_back(msg);
        }
    }
    file.close();
}

DemoFile::~DemoFile()
{
    for (DemoMessage* msg : m_messages) {
        delete msg;
    }
}

void DemoFile::printHeader()
{
    std::cout << "demoprotocol: " << m_header.demoprotocol << std::endl;
    std::cout << "networkprotocol: " << m_header.networkprotocol << std::endl;
    std::cout << "servername: " << m_header.servername << std::endl;
    std::cout << "clientname: " << m_header.clientname << std::endl;
    std::cout << "mapname: " << m_header.mapname << std::endl;
    std::cout << "gamedirectory: " << m_header.gamedirectory << std::endl;
    std::cout << "playback_time: " << m_header.playback_time << std::endl;
    std::cout << "playback_ticks: " << m_header.playback_ticks << std::endl;
    std::cout << "playback_frames: " << m_header.playback_frames << std::endl;
    std::cout << "signonlength: " << m_header.signonlength << std::endl;
    std::cout << std::endl;
}

void DemoFile::printMessages()
{
    for (const DemoMessage* msg : m_messages) {
        std::cout << msg->toString();
        std::cout << std::endl;
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
