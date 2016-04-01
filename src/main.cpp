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

const char* MSG_NAMES[] = {"", "Signon", "Packet", "SyncTick", "ConsoleCmd", "UserCmd", "DataTables", "Stop", "StringTables"};

int main(int argc, char **argv)
{
    const char* filename = "/home/gabriel/Spiele/Steam/Team Fortress 2/tf/crab.dem";
    DemoFile demo(filename);
    
    for (const DemoMessage& msg : demo.getMessages()) {
        std::cout << "Type: " << MSG_NAMES[(int)msg.type] << std::endl;
        std::cout << "Tick: " << msg.tick << std::endl;
        std::cout << "Size: " << msg.data_size << std::endl;
        std::cout << "Data: \n" << bindump(msg.data, msg.data_size) << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
