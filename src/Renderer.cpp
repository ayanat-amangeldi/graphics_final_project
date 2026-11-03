#include "concert/Renderer.hpp"

namespace concert {
Renderer::Renderer(const std::filesystem::path& path)
    :shader_(path/"scene.vert",path/"scene.frag"),cube_(Mesh::cube()) {}
void Renderer::begin(const Camera& camera,int width,int height) {
    gl::Enable(gl::DEPTH_TEST);gl::Enable(gl::CULL_FACE);gl::Disable(gl::BLEND);
    gl::Viewport(0,0,width,height);gl::ClearColor(0.012f,0.018f,0.035f,1);
    gl::Clear(gl::COLOR_BUFFER_BIT|gl::DEPTH_BUFFER_BIT);
    shader_.use();shader_.set("uView",camera.view());
    shader_.set("uProjection",perspective(config::fov,static_cast<float>(width)/height,
                                         config::nearPlane,config::farPlane));
    shader_.set("uCamera",camera.position());
    // Neutral work lights for the scaffold; LightShow owner replaces these with spotlights.
    shader_.set("uLightPosition0",Vec3{-6,9,7});
    shader_.set("uLightPosition1",Vec3{6,7,-2});
    shader_.set("uLightColor0",Vec3{1.0f,0.91f,0.8f});
    shader_.set("uLightColor1",Vec3{0.45f,0.62f,1.0f});
}
void Renderer::box(Vec3 center,Vec3 size,Vec3 color,bool grid,float emission) {
    box(translation(center)*scaling(size),color,grid,emission);
}
void Renderer::box(const Mat4& model,Vec3 color,bool grid,float emission) {
    shader_.use();shader_.set("uModel",model);shader_.set("uColor",color);
    shader_.set("uGrid",grid?1:0);shader_.set("uEmission",emission);cube_.draw();
}
}
