#include <iostream>
#include <stdint.h>

#include "demofile.hpp"
#include "eventparser.hpp"
#include "config.hpp"

int main(int argc, char **argv)
{
    // Initialize global event parser
    g_evtparser = new EventParser();

#ifdef LOAD_EVENT_RESOURCES
    // Load event definitions
    for (size_t res_i = 0; res_i < EVENT_RESOURCES_LEN; res_i++) {
        g_evtparser->loadFile(EVENT_RESOURCES[res_i]);
    }
#endif

#ifdef ENABLE_DEFAULT_DEMO
    DemoFile demo(DEMO_PATH);

    demo.printHeader();
    demo.printMessages();
#endif

    delete g_evtparser;
    return 0;
}
