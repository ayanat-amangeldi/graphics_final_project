#pragma once
#include <cstdint>

namespace concert {
// Input changes these values centrally; modules read them during update/draw.
struct SceneState {
    double animationTime=0;
    int preset=1;
    bool paused=false,focusSinger=false,ledAnimated=true,showHelp=true;
    std::uint64_t confettiRequest=0; // Increment on C; particle module remembers last handled value.
};
}
