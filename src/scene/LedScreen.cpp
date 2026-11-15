#include "concert/Scene.hpp"

namespace concert {

void LedScreen::update(const SceneState& state, float dt) {
    if (state.ledAnimated) {
        elapsed_ += dt;
    }
}

void LedScreen::draw(Renderer& r, const SceneState& state) const {
    r.drawLed(
        config::ledCenter,
        config::ledSize,
        elapsed_,
        state.preset
    );
}

}
    