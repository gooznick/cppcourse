#pragma once 

#include "tracker_structs.h"
#include "telem.h"


struct TrackerManager
{
    TrackerManager(TrackerInput){};
    void Track(Telem&) {};
};