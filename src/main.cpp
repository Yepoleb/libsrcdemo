#include <iostream>
#include <stdint.h>

#include "DemoFile.hpp"

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
