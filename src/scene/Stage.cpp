#include "concert/Scene.hpp"

namespace concert {
void Stage::draw(Renderer& r,const SceneState&) const {
    // TODO teammate 2: replace blockout with stage, band, instruments and detailed trusses.
    r.box({0,-0.1f,8},{40,0.2f,40},{0.065f,0.085f,0.11f},true);
    r.box(config::stageCenter,config::stageSize,{0.11f,0.14f,0.18f});
    r.box({0,0.85f,4.02f},{16,0.08f,0.04f},{0.1f,0.7f,0.85f},false,0.7f);
    for(float z:{-3.0f,3.0f}) {
        r.box({0,7.3f,z},{16,0.25f,0.25f},{0.22f,0.25f,0.3f});
        for(float x:{-7.8f,7.8f})
            r.box({x,4.1f,z},{0.25f,6.2f,0.25f},{0.22f,0.25f,0.3f});
    }
    // Yellow block = singer position marker, feet exactly at the shared origin.
    r.box(config::singerFeet+Vec3{0,0.9f,0},{0.55f,1.8f,0.45f},{0.95f,0.6f,0.15f});
    for(Vec3 feet:{Vec3{-4,1,0},Vec3{4,1,0},Vec3{0,1,-2}})
        r.box(feet+Vec3{0,0.9f,0},{0.55f,1.8f,0.45f},{0.25f,0.38f,0.5f});
    // World orientation markers: red +X, green +Y, blue +Z at the stage origin.
    r.box({1,1.02f,0},{2,0.04f,0.04f},{1,0.12f,0.12f},false,0.5f);
    r.box({0,1.52f,0},{0.04f,1,0.04f},{0.12f,1,0.2f},false,0.5f);
    r.box({0,1.02f,1},{0.04f,0.04f,2},{0.15f,0.35f,1},false,0.5f);
}
}
