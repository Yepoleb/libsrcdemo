#pragma once

#define JOIN_STRINGS(a,b) a b
#define ARRAY_LEN(array) (sizeof(array) / sizeof(*array))

#define USER_HOME "/home/gaben/"
#define TF2_PATH JOIN_STRINGS(USER_HOME, ".steam/steamapps/common/Team Fortress 2/")

// Enable loading event definitions from resource files
//#define LOAD_EVENT_RESOURCES

// Enable stringtables parsing
//#define ENABLE_STRINGTABLESMSG

// Enable a default demo path, intended for faster debugging
//#define ENABLE_DEFAULT_DEMO

// --- Debug options ---
// Print packet after parsing
//#define DEBUG_PACKETS
// Print tick after parsing
//#define DEBUG_TICKS

#ifdef ENABLE_DEFAULT_DEMO
const char* DEMO_PATH = JOIN_STRINGS(TF2_PATH, "tf/send_to_higps.dem");
#endif

#ifdef LOAD_EVENT_RESOURCES
// Keep these in order!
const char* EVENT_RESOURCES[] = {
    JOIN_STRINGS(TF2_PATH, "hl2/resource/serverevents.res"),
    JOIN_STRINGS(TF2_PATH, "hl2/resource/gameevents.res"),
    JOIN_STRINGS(USER_HOME, "libsrcdemo/data/modevents.res"),
    JOIN_STRINGS(TF2_PATH, "hl2/resource/hltvevents.res"),
    JOIN_STRINGS(TF2_PATH, "hl2/resource/replayevents.res")
};
const size_t EVENT_RESOURCES_LEN = ARRAY_LEN(EVENT_RESOURCES);
#endif

