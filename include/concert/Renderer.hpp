#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include <vector>

namespace concert {
struct Spotlight {
    Vec3 position, direction, color;
    float innerCos = 0, outerCos = 0, intensity = 0, beamOpacity = 0;
};
using Spotlights = std::array<Spotlight, config::rigAnchors.size()>;

struct CrowdInstance {
    Mat4 model;
    float color[4]{}; // RGB clothing color and stable animation phase.
};
struct ConfettiInstance {
    Vec3 position,color;
    float sizeX=0,sizeY=0,rotation=0;
};
class Renderer {
public:
    explicit Renderer(const std::filesystem::path& shaderDir);
    ~Renderer();
    void begin(const Camera& camera,int width,int height);
    void box(Vec3 center,Vec3 size,Vec3 color,bool grid=false,float emission=0);
    void box(const Mat4& model,Vec3 color,bool grid=false,float emission=0);
    void drawCrowd(const std::vector<CrowdInstance>& instances,float animationTime,float beatPulse);
    void drawConfetti(const std::vector<ConfettiInstance>& instances);
    void drawLed(Vec3 center, Vec3 size, float time, int preset);
    void setSpotlights(const Spotlights& lights);
    void drawBeams(const Spotlights& lights);
private:
    Shader beamShader_;
    Mesh beamCone_;
    Vec3 cameraPosition_;
    Shader shader_;
    Shader ledShader_;
    Mesh cube_;
    Shader crowdShader_;
    Shader confettiShader_;
    gl::UInt crowdVao_=0,crowdVbo_=0,crowdEbo_=0,crowdInstanceBuffer_=0;
    gl::Size crowdIndexCount_=0;
    gl::Size crowdInstanceCount_=0;
    gl::UInt confettiVao_=0,confettiVbo_=0,confettiEbo_=0,confettiInstanceBuffer_=0;
    Mat4 view_=Mat4::identity(),projection_=Mat4::identity();
};
}
