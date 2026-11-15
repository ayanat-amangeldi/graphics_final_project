#include "concert/Renderer.hpp"
#include <cstddef>
#include <vector>

namespace concert {
Renderer::Renderer(const std::filesystem::path& path)
    :shader_(path/"scene.vert",path/"scene.frag"),cube_(Mesh::cube()),
     crowdShader_(path/"crowd.vert",path/"crowd.frag"),
     confettiShader_(path/"confetti.vert",path/"confetti.frag") {
    // One combined low-poly spectator mesh: head, torso, arms and legs are
    // separate box shapes packed into this single indexed mesh.
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
    appendBox({0,0.91f,0},{0.38f,0.54f,0.24f},0); // torso
    appendBox({0,1.49f,0},{0.32f,0.34f,0.32f},3); // head
    appendBox({-0.29f,0.94f,0},{0.17f,0.48f,0.19f},1); // left arm
    appendBox({ 0.29f,0.94f,0},{0.17f,0.48f,0.19f},2); // right arm
    appendBox({-0.115f,0.31f,0},{0.15f,0.58f,0.20f},4); // left leg
    appendBox({ 0.115f,0.31f,0},{0.15f,0.58f,0.20f},4); // right leg
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

    // A single unit paper rectangle is repeated through the instance buffer.
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
void Renderer::drawCrowd(const std::vector<CrowdInstance>& instances,float animationTime) {
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
    gl::Disable(gl::CULL_FACE); // Keep paper visible if viewed from either side.
    gl::BindVertexArray(confettiVao_);
    gl::DrawElementsInstanced(gl::TRIANGLES,6,gl::UNSIGNED_INT,nullptr,
                              static_cast<gl::Size>(instances.size()));
    gl::BindVertexArray(0);
    gl::Enable(gl::CULL_FACE);
}
}
