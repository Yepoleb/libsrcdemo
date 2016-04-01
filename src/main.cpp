#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>
#include <sstream>
#include <iomanip>

#include "DemoFile.hpp"

std::string bindump(const char* data, const size_t& length)
{
    std::stringstream ss;

    for (size_t i = 0; i < length; i++) {
        uint8_t byte = static_cast<uint8_t>(data[i]);
        ss << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)byte << " ";
        if ((i+1) % 16 == 0) {
            ss << "\n";
        }
    }
    return ss.str();
}

const char SAFE_CHARS[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\\'()*+,-./:;<=>?@[\\]^_`{|}~ ";

std::string escape(std::string str)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2);
    for (size_t i = 0; i < str.size(); i++) {
        char c = str.at(i);
        bool safe = false;
        for (size_t si = 0; si < sizeof(SAFE_CHARS); si++) {
            if (c == SAFE_CHARS[si])
                safe = true;
        }
        if (safe) {
            ss << c;
        } else {
            ss << "\\x" << (unsigned int)(unsigned char)c;
        }
    }
    return ss.str();
}

const char* MSG_NAMES[] = {"", "Signon", "Packet", "SyncTick", "ConsoleCmd", "UserCmd", "DataTables", "Stop", "StringTables"};

int main(int argc, char **argv)
{
    const char* filename = "/home/gabriel/Spiele/Steam/Team Fortress 2/tf/crab.dem";
    DemoFile demo(filename);
    
    for (const DemoMessage* msg : demo.getMessages()) {
        std::cout << msg->toString();
        std::cout << std::endl;
    }

    return 0;
}
