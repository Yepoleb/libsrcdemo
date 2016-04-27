#pragma once
#include <string>
#include <stdint.h>
#include <vector>
#include <exception>

#include "DemoMessage.hpp"

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
    ~DemoFile();
    DemoHeader getHeader() const { return m_header; };
    std::vector<DemoMessage*> getMessages() const { return m_messages; };
    void printHeader();
    void printMessages();
private:
    DemoHeader m_header;
    std::vector<DemoMessage*> m_messages;
};

char* ReadBytes(std::istream& stream, const size_t& length);
std::string ReadString(std::istream& stream, const size_t& length);
int8_t ReadInt8(std::istream& stream);
int32_t ReadInt32(std::istream& stream);
float ReadFloat(std::istream& stream);
