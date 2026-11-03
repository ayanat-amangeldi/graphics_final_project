#include "concert/Scene.hpp"

namespace concert {
void LightShow::update(const SceneState&,float) {
    // TODO teammate 1: calculate rig poses and spotlight directions here.
}
void LightShow::configure(Renderer&,const SceneState&) const {
    // TODO teammate 1: upload lighting before any scene geometry is drawn.
}
void LightShow::draw(Renderer& r,const SceneState& state) const {
    // TODO teammate 1: actual spotlight uniforms, cones, presets and singer focus.
    // These boxes only test shared attachment points and parent-child transforms.
    const Vec3 colors[]={{0.3f,0.35f,1},{1,0.15f,0.6f},{1,0.55f,0.1f}};
    Vec3 color=colors[state.preset-1];
    for(Vec3 anchor:config::rigAnchors) {
        Mat4 parent=translation(anchor)*rotationY(static_cast<float>(state.animationTime)*0.6f);
        r.box(translation(anchor)*scaling({0.18f,0.35f,0.18f}),{0.3f,0.33f,0.4f});
        Mat4 head=parent*translation({0,-0.35f,0})*rotationX(radians(20));
        r.box(head*scaling({0.42f,0.5f,0.42f}),color);
        r.box(head*translation({0,-0.26f,0})*scaling({0.3f,0.02f,0.3f}),color,false,1);
    }
}
}
