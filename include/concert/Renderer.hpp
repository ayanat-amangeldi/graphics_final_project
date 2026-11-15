#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

namespace concert {
class Renderer {
public:
    explicit Renderer(const std::filesystem::path& shaderDir);
    void begin(const Camera& camera,int width,int height);
    void box(Vec3 center,Vec3 size,Vec3 color,bool grid=false,float emission=0);
    void box(const Mat4& model,Vec3 color,bool grid=false,float emission=0);
    void drawLed(Vec3 center, Vec3 size, float time, int preset);
private:
    Shader shader_;
    Shader ledShader_;
    Mesh cube_;
};
}
