#include <stdint.h>
#include <iostream>

#include "demofile.hpp"
#include "eventparser.hpp"
#include "config.hpp"

int main(int argc, char **argv)
{
    // Initialize global event parser
    g_evtparser = new EventParser();

    DemoFile demo;
    if (argc == 2) {
        demo.load(argv[1]);
    } else {
#ifdef ENABLE_DEFAULT_DEMO
        demo.load(DEMO_PATH);
#else
        std::cerr << "Usage: " << argv[0] << " <file.dem>" << std::endl;
        return 1;
#endif
    }

    demo.printHeader();
    demo.printMessages();

    delete g_evtparser;
    return 0;
}
