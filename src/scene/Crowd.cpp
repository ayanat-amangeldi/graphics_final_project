#include "concert/Scene.hpp"
#include "concert/Config.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

namespace concert {
namespace {
constexpr std::size_t confettiPerBurst=240;
constexpr std::size_t maxConfetti=1200;
float randomUnit(std::uint32_t& state) {
    state^=state<<13;state^=state>>17;state^=state<<5;
    return static_cast<float>(state>>8)*(1.0f/16777215.0f);
}
float randomRange(std::uint32_t& state,float low,float high) {
    return low+(high-low)*randomUnit(state);
}
const std::vector<CrowdInstance>& audienceInstances() {
    static const std::vector<CrowdInstance> instances=[] {
        static const Vec3 palette[]={
            {0.16f,0.38f,0.78f}, // blue
            {0.78f,0.20f,0.24f}, // red
            {0.86f,0.65f,0.16f}, // yellow
            {0.18f,0.55f,0.34f}, // green
            {0.48f,0.27f,0.68f}, // purple
            {0.88f,0.40f,0.16f}  // orange
        };
        std::vector<CrowdInstance> result;
        result.reserve(static_cast<std::size_t>(config::crowdColumns*config::crowdRows));
        const float xStep=(config::crowdMaxX-config::crowdMinX)/(config::crowdColumns-1);
        const float zStep=(config::crowdMaxZ-config::crowdMinZ)/(config::crowdRows-1);
        for(int row=0;row<config::crowdRows;++row) {
            for(int column=0;column<config::crowdColumns;++column) {
                float x=config::crowdMinX+column*xStep;
                float z=config::crowdMinZ+row*zStep;
                const int index=row*config::crowdColumns+column;
                Vec3 color=palette[(row*7+column*5)%6];
                float phase=static_cast<float>((index*37)%120)*radians(3.0f);
                // Slightly scale the model to leave visible gaps between neighbors.
                // Its front is local +Z; rotate it toward the stage (-Z).
                Mat4 model=translation({x,0,z})*rotationY(radians(180))*scaling({0.88f,0.88f,0.88f});
                result.push_back({model,{color.x,color.y,color.z,phase}});
            }
        }
        return result;
    }();
    return instances;
}
}
Crowd::Crowd() {
    particles_.reserve(maxConfetti);
    renderParticles_.reserve(maxConfetti);
}
void Crowd::spawnConfetti() {
    static const Vec3 palette[]={
        {1.0f,0.18f,0.22f},{1.0f,0.72f,0.10f},{0.18f,0.88f,0.40f},
        {0.12f,0.72f,1.0f},{0.78f,0.28f,1.0f},{1.0f,0.38f,0.10f},
        {1.0f,0.42f,0.76f}
    };
    if(particles_.size()+confettiPerBurst>maxConfetti) {
        const std::size_t removeCount=particles_.size()+confettiPerBurst-maxConfetti;
        particles_.erase(particles_.begin(),particles_.begin()+static_cast<std::ptrdiff_t>(removeCount));
    }
    for(std::size_t i=0;i<confettiPerBurst;++i) {
        const Vec3 origin=config::confettiOrigins[i%config::confettiOrigins.size()];
        const Vec3 color=palette[static_cast<std::size_t>(randomUnit(randomState_)*7.0f)%7];
        Particle particle;
        particle.position={origin.x+randomRange(randomState_,-0.55f,0.55f),
                           origin.y+randomRange(randomState_,-0.25f,0.45f),origin.z};
        particle.velocity={randomRange(randomState_,-2.6f,2.6f),
                           randomRange(randomState_,2.0f,5.5f),
                           randomRange(randomState_,1.0f,4.0f)};
        particle.color=color;
        particle.sizeX=randomRange(randomState_,0.08f,0.18f);
        particle.sizeY=randomRange(randomState_,0.045f,0.11f);
        particle.rotation=randomRange(randomState_,-3.14f,3.14f);
        particle.rotationSpeed=randomRange(randomState_,-6.0f,6.0f);
        particle.wobblePhase=randomRange(randomState_,0.0f,6.28f);
        particles_.push_back(particle);
    }
}
void Crowd::update(const SceneState& state,float dt) {
    if(state.confettiRequest!=lastConfettiRequest_) {
        lastConfettiRequest_=state.confettiRequest;
        spawnConfetti();
    }
    animationTime_+=dt;
    for(Particle& particle:particles_) {
        particle.age+=dt;
        particle.velocity.y-=3.0f*dt;
        particle.velocity.x+=0.18f*std::sin(particle.age*1.4f+particle.wobblePhase)*dt;
        particle.velocity.z+=0.10f*std::cos(particle.age+particle.wobblePhase)*dt;
        particle.position=particle.position+particle.velocity*dt;
        particle.rotation+=particle.rotationSpeed*dt;
    }
    particles_.erase(std::remove_if(particles_.begin(),particles_.end(),[](const Particle& p) {
        return p.position.y < -0.5f || p.age > 12.0f;
    }),particles_.end());
}
void Crowd::draw(Renderer& renderer,const SceneState&) const {
    renderer.drawCrowd(audienceInstances(),animationTime_);
    renderParticles_.clear();
    for(const Particle& particle:particles_)
        renderParticles_.push_back({particle.position,particle.color,particle.sizeX,
                                    particle.sizeY,particle.rotation});
    renderer.drawConfetti(renderParticles_);
}
}
