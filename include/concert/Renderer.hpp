#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include <vector>

namespace concert {
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
    void drawCrowd(const std::vector<CrowdInstance>& instances,float animationTime);
    void drawConfetti(const std::vector<ConfettiInstance>& instances);
private:
    Shader shader_;
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
