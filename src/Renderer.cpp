#include "concert/Renderer.hpp"
#include <cstddef>
#include <vector>

namespace concert {
namespace {
Mesh makeBeamCone() {
    constexpr unsigned int segments = 32;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < segments; ++i) {
        const float angle = 2 * pi * i / segments;
        const Vec3 rim{std::cos(angle), -1, std::sin(angle)};
        const Vec3 normal = normalize(Vec3{rim.x, 1, rim.z});
        vertices.push_back({{0, 0, 0}, normal, 0, 0});
        vertices.push_back({rim, normal, 0, 1});
    }
    for (unsigned int i = 0; i < segments; ++i) {
        const unsigned int next = (i + 1) % segments;
        indices.insert(indices.end(), {2 * i, 2 * i + 1, 2 * next + 1});
    }
    return Mesh(vertices, indices);
}

Mat4 beamTransform(const Spotlight& light) {
    const Vec3 axis = light.direction;
    const Vec3 side = normalize(cross(axis, {0, 0, 1}));
    const Vec3 back = cross(axis, side);
    const float length = (light.position.y - 1.0f) / -axis.y;
    const float radius = length * std::tan(std::acos(light.outerCos));
    const Vec3 x = side * radius;
    const Vec3 y = axis * -length;
    const Vec3 z = back * radius;
    const Vec3 p = light.position;
    return {{x.x,x.y,x.z,0, y.x,y.y,y.z,0, z.x,z.y,z.z,0, p.x,p.y,p.z,1}};
}
}
Renderer::Renderer(const std::filesystem::path& path)
    :beamShader_(path/"scene.vert",path/"beam.frag"),beamCone_(makeBeamCone()),
     shader_(path/"scene.vert",path/"scene.frag"),
     ledShader_(path/"led.vert",path/"led.frag"),cube_(Mesh::cube()),
     crowdShader_(path/"crowd.vert",path/"crowd.frag"),
     confettiShader_(path/"confetti.vert",path/"confetti.frag") {
    struct CrowdVertex { Vec3 position,normal;float u,v; };
    std::vector<CrowdVertex> vertices;
    std::vector<unsigned int> indices;
    const Vec3 normals[]={{0,0,1},{0,0,-1},{1,0,0},{-1,0,0},{0,1,0},{0,-1,0}};
    const Vec3 tangents[]={{1,0,0},{-1,0,0},{0,0,-1},{0,0,1},{1,0,0},{1,0,0}};
    auto appendBox=[&](Vec3 center,Vec3 size,float part) {
        for(int face=0;face<6;++face) {
            Vec3 n=normals[face],t=tangents[face],b=cross(n,t);
            unsigned int start=static_cast<unsigned int>(vertices.size());
            const Vec3 corners[]={(n-t-b)*0.5f,(n+t-b)*0.5f,
                                  (n+t+b)*0.5f,(n-t+b)*0.5f};
            for(Vec3 p:corners)
                vertices.push_back({center+Vec3{p.x*size.x,p.y*size.y,p.z*size.z},n,part,0});
            for(unsigned int i:{0u,1u,2u,0u,2u,3u}) indices.push_back(start+i);
        }
    };
    appendBox({0,0.91f,0},{0.38f,0.54f,0.24f},0);
    appendBox({0,1.49f,0},{0.32f,0.34f,0.32f},3);
    appendBox({-0.29f,0.94f,0},{0.17f,0.48f,0.19f},1);
    appendBox({ 0.29f,0.94f,0},{0.17f,0.48f,0.19f},2);
    appendBox({-0.115f,0.31f,0},{0.15f,0.58f,0.20f},4);
    appendBox({ 0.115f,0.31f,0},{0.15f,0.58f,0.20f},4);
    crowdIndexCount_=static_cast<gl::Size>(indices.size());

    gl::GenVertexArrays(1,&crowdVao_);
    gl::GenBuffers(1,&crowdVbo_);
    gl::GenBuffers(1,&crowdEbo_);
    gl::GenBuffers(1,&crowdInstanceBuffer_);
    gl::BindVertexArray(crowdVao_);
    gl::BindBuffer(gl::ARRAY_BUFFER,crowdVbo_);
    gl::BufferData(gl::ARRAY_BUFFER,static_cast<gl::SizePtr>(vertices.size()*sizeof(CrowdVertex)),
                   vertices.data(),gl::STATIC_DRAW);
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER,crowdEbo_);
    gl::BufferData(gl::ELEMENT_ARRAY_BUFFER,static_cast<gl::SizePtr>(indices.size()*sizeof(unsigned int)),
                   indices.data(),gl::STATIC_DRAW);
    gl::EnableVertexAttribArray(0);
    gl::VertexAttribPointer(0,3,gl::FLOAT,gl::FALSE_VALUE,sizeof(CrowdVertex),nullptr);
    gl::EnableVertexAttribArray(1);
    gl::VertexAttribPointer(1,3,gl::FLOAT,gl::FALSE_VALUE,sizeof(CrowdVertex),
                            reinterpret_cast<void*>(offsetof(CrowdVertex,normal)));
    gl::EnableVertexAttribArray(2);
    gl::VertexAttribPointer(2,2,gl::FLOAT,gl::FALSE_VALUE,sizeof(CrowdVertex),
                            reinterpret_cast<void*>(offsetof(CrowdVertex,u)));
    gl::BindBuffer(gl::ARRAY_BUFFER,crowdInstanceBuffer_);
    for(gl::UInt column=0;column<4;++column) {
        gl::UInt location=3+column;
        gl::EnableVertexAttribArray(location);
        gl::VertexAttribPointer(location,4,gl::FLOAT,gl::FALSE_VALUE,sizeof(CrowdInstance),
                                reinterpret_cast<void*>(offsetof(CrowdInstance,model)+
                                                        sizeof(float)*4*column));
        gl::VertexAttribDivisor(location,1);
    }
    constexpr gl::UInt styleLocation=7;
    gl::EnableVertexAttribArray(styleLocation);
    gl::VertexAttribPointer(styleLocation,4,gl::FLOAT,gl::FALSE_VALUE,sizeof(CrowdInstance),
                            reinterpret_cast<void*>(offsetof(CrowdInstance,color)));
    gl::VertexAttribDivisor(styleLocation,1);
    gl::BindVertexArray(0);

    const float quadVertices[]={-0.5f,-0.5f, 0.5f,-0.5f, 0.5f,0.5f, -0.5f,0.5f};
    const unsigned int quadIndices[]={0,1,2,0,2,3};
    gl::GenVertexArrays(1,&confettiVao_);
    gl::GenBuffers(1,&confettiVbo_);
    gl::GenBuffers(1,&confettiEbo_);
    gl::GenBuffers(1,&confettiInstanceBuffer_);
    gl::BindVertexArray(confettiVao_);
    gl::BindBuffer(gl::ARRAY_BUFFER,confettiVbo_);
    gl::BufferData(gl::ARRAY_BUFFER,sizeof(quadVertices),quadVertices,gl::STATIC_DRAW);
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER,confettiEbo_);
    gl::BufferData(gl::ELEMENT_ARRAY_BUFFER,sizeof(quadIndices),quadIndices,gl::STATIC_DRAW);
    gl::EnableVertexAttribArray(0);
    gl::VertexAttribPointer(0,2,gl::FLOAT,gl::FALSE_VALUE,2*sizeof(float),nullptr);
    gl::BindBuffer(gl::ARRAY_BUFFER,confettiInstanceBuffer_);
    gl::EnableVertexAttribArray(1);
    gl::VertexAttribPointer(1,3,gl::FLOAT,gl::FALSE_VALUE,sizeof(ConfettiInstance),
                            reinterpret_cast<void*>(offsetof(ConfettiInstance,position)));
    gl::VertexAttribDivisor(1,1);
    gl::EnableVertexAttribArray(2);
    gl::VertexAttribPointer(2,3,gl::FLOAT,gl::FALSE_VALUE,sizeof(ConfettiInstance),
                            reinterpret_cast<void*>(offsetof(ConfettiInstance,color)));
    gl::VertexAttribDivisor(2,1);
    gl::EnableVertexAttribArray(3);
    gl::VertexAttribPointer(3,2,gl::FLOAT,gl::FALSE_VALUE,sizeof(ConfettiInstance),
                            reinterpret_cast<void*>(offsetof(ConfettiInstance,sizeX)));
    gl::VertexAttribDivisor(3,1);
    gl::EnableVertexAttribArray(4);
    gl::VertexAttribPointer(4,1,gl::FLOAT,gl::FALSE_VALUE,sizeof(ConfettiInstance),
                            reinterpret_cast<void*>(offsetof(ConfettiInstance,rotation)));
    gl::VertexAttribDivisor(4,1);
    gl::BindVertexArray(0);
}
Renderer::~Renderer() {
    gl::DeleteBuffers(1,&crowdInstanceBuffer_);
    gl::DeleteBuffers(1,&crowdEbo_);
    gl::DeleteBuffers(1,&crowdVbo_);
    gl::DeleteVertexArrays(1,&crowdVao_);
    gl::DeleteBuffers(1,&confettiInstanceBuffer_);
    gl::DeleteBuffers(1,&confettiEbo_);
    gl::DeleteBuffers(1,&confettiVbo_);
    gl::DeleteVertexArrays(1,&confettiVao_);
}
void Renderer::begin(const Camera& camera,int width,int height) {
    gl::Enable(gl::DEPTH_TEST);gl::Enable(gl::CULL_FACE);gl::Disable(gl::BLEND);
    gl::Viewport(0,0,width,height);gl::ClearColor(0.012f,0.018f,0.035f,1);
    gl::Clear(gl::COLOR_BUFFER_BIT|gl::DEPTH_BUFFER_BIT);
    view_=camera.view();
    projection_=perspective(config::fov,static_cast<float>(width)/height,
                            config::nearPlane,config::farPlane);
    shader_.use();shader_.set("uView",view_);
    shader_.set("uProjection",projection_);
    cameraPosition_=camera.position();
    shader_.set("uCamera",cameraPosition_);
    ledShader_.use();
    ledShader_.set("uView", view_);
    ledShader_.set("uProjection", projection_);
}
void Renderer::setSpotlights(const Spotlights& lights) {
    shader_.use();
    for (std::size_t i = 0; i < lights.size(); ++i) {
        const auto& light = lights[i];
        const std::string prefix = "uSpotlights[" + std::to_string(i) + "].";
        shader_.set(prefix + "position", light.position);
        shader_.set(prefix + "direction", light.direction);
        shader_.set(prefix + "color", light.color);
        shader_.set(prefix + "params", Vec3{light.outerCos, light.innerCos, light.intensity});
    }
}

void Renderer::drawBeams(const Spotlights& lights) {
    gl::Enable(gl::BLEND);
    gl::BlendFunc(gl::SRC_ALPHA, gl::ONE);
    gl::DepthMask(gl::FALSE_VALUE);
    gl::Disable(gl::CULL_FACE);
    beamShader_.use();
    beamShader_.set("uView", view_);
    beamShader_.set("uProjection", projection_);
    beamShader_.set("uCamera", cameraPosition_);
    for (const auto& light : lights) {
        beamShader_.set("uModel", beamTransform(light));
        beamShader_.set("uColor", light.color);
        beamShader_.set("uOpacity", light.beamOpacity);
        beamCone_.draw();
    }
    gl::Enable(gl::CULL_FACE);
    gl::DepthMask(gl::TRUE_VALUE);
    gl::Disable(gl::BLEND);
}

void Renderer::box(Vec3 center,Vec3 size,Vec3 color,bool grid,float emission) {
    box(translation(center)*scaling(size),color,grid,emission);
}
void Renderer::box(const Mat4& model,Vec3 color,bool grid,float emission) {
    shader_.use();shader_.set("uModel",model);shader_.set("uColor",color);
    shader_.set("uGrid",grid?1:0);shader_.set("uEmission",emission);cube_.draw();
}
void Renderer::drawCrowd(const std::vector<CrowdInstance>& instances,float animationTime,float beatPulse) {
    if(instances.empty()) return;
    if(crowdInstanceCount_!=static_cast<gl::Size>(instances.size())) {
        gl::BindBuffer(gl::ARRAY_BUFFER,crowdInstanceBuffer_);
        gl::BufferData(gl::ARRAY_BUFFER,
                       static_cast<gl::SizePtr>(instances.size()*sizeof(CrowdInstance)),
                       instances.data(),gl::STATIC_DRAW);
        crowdInstanceCount_=static_cast<gl::Size>(instances.size());
    }
    crowdShader_.use();
    crowdShader_.set("uView",view_);
    crowdShader_.set("uProjection",projection_);
    crowdShader_.set("uAnimationTime",animationTime);
    crowdShader_.set("uBeatPulse",beatPulse);
    gl::BindVertexArray(crowdVao_);
    gl::DrawElementsInstanced(gl::TRIANGLES,crowdIndexCount_,gl::UNSIGNED_INT,nullptr,
                              static_cast<gl::Size>(instances.size()));
    gl::BindVertexArray(0);
}
void Renderer::drawConfetti(const std::vector<ConfettiInstance>& instances) {
    if(instances.empty()) return;
    gl::BindBuffer(gl::ARRAY_BUFFER,confettiInstanceBuffer_);
    gl::BufferData(gl::ARRAY_BUFFER,
                   static_cast<gl::SizePtr>(instances.size()*sizeof(ConfettiInstance)),
                   instances.data(),gl::DYNAMIC_DRAW);
    confettiShader_.use();
    confettiShader_.set("uView",view_);
    confettiShader_.set("uProjection",projection_);
    gl::Disable(gl::CULL_FACE);
    gl::BindVertexArray(confettiVao_);
    gl::DrawElementsInstanced(gl::TRIANGLES,6,gl::UNSIGNED_INT,nullptr,
                              static_cast<gl::Size>(instances.size()));
    gl::BindVertexArray(0);
    gl::Enable(gl::CULL_FACE);
}

void Renderer::drawLed(Vec3 center, Vec3 size,
                       float time, int preset) {
    ledShader_.use();

    Mat4 model = translation(center) * scaling(size);

    ledShader_.set("uModel", model);
    ledShader_.set("uTime", time);
    ledShader_.set("uPreset", preset);

    cube_.draw();
}

}
