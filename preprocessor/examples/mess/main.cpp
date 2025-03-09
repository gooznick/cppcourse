#include "telem.h"
#include "tracker_api.h"

int main()
{
    TrackerInput input;
    TrackerManager tracker_manager(input);
    Telem telem;
    tracker_manager.Track(telem);

    return 0;
}