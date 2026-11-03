#include "concert/Scene.hpp"

namespace concert {
void LedScreen::draw(Renderer& r,const SceneState&) const {
    // TODO teammate 4: add led.vert/led.frag, animated patterns, own shader clock.
    // Static screen marks dimensions; L currently changes only the shared state.
    r.box(config::ledCenter,config::ledSize,{0.075f,0.12f,0.2f},false,0.5f);
}
}
