#pragma once
#include "Renderer.hpp"
#include "State.hpp"
#include <vector>

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
    Crowd();
    void update(const SceneState& state,float dt);
    void draw(Renderer& renderer,const SceneState& state) const;
private:
    struct Particle {
        Vec3 position,velocity,color;
        float sizeX=0,sizeY=0,rotation=0,rotationSpeed=0,age=0,wobblePhase=0;
    };
    void spawnConfetti();
    std::uint64_t lastConfettiRequest_=0;
    float animationTime_=0;
    std::uint32_t randomState_=0x6D2B79F5u;
    std::vector<Particle> particles_;
    mutable std::vector<ConfettiInstance> renderParticles_;
};
class LedScreen {
public: void draw(Renderer& renderer,const SceneState& state) const;
};
class Scene {
public:
    void update(const SceneState& state,float dt) {
        lights_.update(state,dt);crowd_.update(state,dt);
    }
    void draw(Renderer& renderer,const SceneState& state) const {
        lights_.configure(renderer,state);
        stage_.draw(renderer,state);lights_.draw(renderer,state);
        crowd_.draw(renderer,state);led_.draw(renderer,state);
    }
private: Stage stage_;LightShow lights_;Crowd crowd_;LedScreen led_;
};
}
