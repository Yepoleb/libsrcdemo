#include <iostream>
#include <stdint.h>

#include "DemoFile.hpp"
#include "EventParser.hpp"
#include "paths.hpp"

int main(int argc, char **argv)
{
    // Initialize global event parser
    g_evtparser = new EventParser();
    // Load event definitions
    for (size_t res_i = 0; res_i < EVENT_RESOURCES_LEN; res_i++) {
        g_evtparser->loadFile(EVENT_RESOURCES[res_i]);
    }

    DemoFile demo(DEMO_PATH);

    demo.printHeader();
    demo.printMessages();

    delete g_evtparser;
    return 0;
}
