#pragma once
#include "Renderer.hpp"
#include "State.hpp"

namespace concert {
// Each teammate owns one module and its source file. No GLFW calls in modules.
class Stage {
public: void draw(Renderer& renderer,const SceneState& state) const;
};
class LightShow {
public:
    void update(const SceneState& state,float dt);
    void configure(Renderer& renderer,const SceneState& state) const;
    void draw(Renderer& renderer,const SceneState& state) const;
};
class Crowd {
public:
    void update(const SceneState& state,float dt);
    void draw(Renderer& renderer,const SceneState& state) const;
private: std::uint64_t lastConfettiRequest_=0;
};

class LedScreen {
public:
    void update(const SceneState& state, float dt);
    void draw(Renderer& renderer, const SceneState& state) const;

private:
    float elapsed_ = 0.0f;
};

class Scene {
public:
    void update(const SceneState& state,float dt) {
    lights_.update(state,dt);
    crowd_.update(state,dt);
    led_.update(state,dt);
  }
    void draw(Renderer& renderer,const SceneState& state) const {
        lights_.configure(renderer,state);
        stage_.draw(renderer,state);lights_.draw(renderer,state);
        crowd_.draw(renderer,state);led_.draw(renderer,state);
    }
private: Stage stage_;LightShow lights_;Crowd crowd_;LedScreen led_;
};
}
