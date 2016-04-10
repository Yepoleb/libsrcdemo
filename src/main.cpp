#include <iostream>
#include <stdint.h>

#include "DemoFile.hpp"
#include "helpers.hpp"

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
