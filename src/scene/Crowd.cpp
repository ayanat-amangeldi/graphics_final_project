#include "concert/Scene.hpp"
#include <iostream>

namespace concert {
void Crowd::update(const SceneState& state,float dt) {
    if(state.confettiRequest!=lastConfettiRequest_) {
        lastConfettiRequest_=state.confettiRequest;
        std::cout<<"[Crowd stub] Confetti request received; particle system is not implemented.\n";
    }
    (void)dt; // TODO teammate 3: update particles using dt; dt is zero while paused.
}
void Crowd::draw(Renderer& r,const SceneState&) const {
    // TODO teammate 3: replace three position markers with a 12x10 instanced crowd.
    for(float x:{-6.0f,0.0f,6.0f})
        r.box({x,0.9f,9},{0.55f,1.8f,0.45f},{0.22f,0.42f,0.4f});
}
}
