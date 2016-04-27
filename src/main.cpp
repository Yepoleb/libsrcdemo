#include <iostream>
#include <stdint.h>

#include "DemoFile.hpp"
#include "EventParser.hpp"
#include "helpers.hpp"

int main(int argc, char **argv)
{
    //const char* filename = "/home/gabriel/Spiele/Steam/Team Fortress 2/tf/crab.dem";
    //const char* filename = "/home/gabriel/Spiele/Steam/Team Fortress 2/tf/steel2.dem";
    const char* filename = "/home/gabriel/Spiele/Steam/Team Fortress 2/tf/replay/client/replays/20160125-032359-cp_dustbowl_18.dem";
    // Initialize global event parser
    g_evtparser = new EventParser();
    g_evtparser->loadFile("/home/gabriel/Spiele/Steam/Team Fortress 2/hl2/resource/serverevents.res");
    g_evtparser->loadFile("/home/gabriel/Spiele/Steam/Team Fortress 2/hl2/resource/gameevents.res");
    g_evtparser->loadFile("/home/gabriel/temp/tf2/tf2_misc_dir./resource/modevents.res");
    g_evtparser->loadFile("/home/gabriel/Spiele/Steam/Team Fortress 2/hl2/resource/replayevents.res");
    //g_evtparser->printEvents();

    DemoFile demo(filename);

    demo.printHeader();
    demo.printMessages();

    delete g_evtparser;
    g_evtparser = nullptr;
    return 0;
}
