#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"
#include "State.hpp"
#include <memory>
#include <string>

namespace concert {
class Hud {
public:
    explicit Hud(const std::filesystem::path& shaders):shader_(shaders/"hud.vert",shaders/"hud.frag") {}
    void draw(int width,int height,float fps,const SceneState& state);
private:
    Shader shader_;
    std::unique_ptr<Mesh> text_,background_;
    std::string lastText_;
    int lastWidth_=0,lastHeight_=0;
};
}
