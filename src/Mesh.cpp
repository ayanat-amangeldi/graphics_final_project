#include "concert/Mesh.hpp"
#include <cstddef>

namespace concert {
Mesh::Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indices)
    :count_(static_cast<gl::Size>(indices.size())) {
    gl::GenVertexArrays(1,&vao_);gl::GenBuffers(1,&vbo_);gl::GenBuffers(1,&ebo_);
    gl::BindVertexArray(vao_);gl::BindBuffer(gl::ARRAY_BUFFER,vbo_);
    gl::BufferData(gl::ARRAY_BUFFER,static_cast<gl::SizePtr>(vertices.size()*sizeof(Vertex)),
                   vertices.data(),gl::STATIC_DRAW);
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER,ebo_);
    gl::BufferData(gl::ELEMENT_ARRAY_BUFFER,static_cast<gl::SizePtr>(indices.size()*sizeof(unsigned int)),
                   indices.data(),gl::STATIC_DRAW);
    gl::EnableVertexAttribArray(0);
    gl::VertexAttribPointer(0,3,gl::FLOAT,gl::FALSE_VALUE,sizeof(Vertex),
                            reinterpret_cast<void*>(offsetof(Vertex,position)));
    gl::EnableVertexAttribArray(1);
    gl::VertexAttribPointer(1,3,gl::FLOAT,gl::FALSE_VALUE,sizeof(Vertex),
                            reinterpret_cast<void*>(offsetof(Vertex,normal)));
    gl::EnableVertexAttribArray(2);
    gl::VertexAttribPointer(2,2,gl::FLOAT,gl::FALSE_VALUE,sizeof(Vertex),
                            reinterpret_cast<void*>(offsetof(Vertex,u)));
    gl::BindVertexArray(0);
}
Mesh::~Mesh() {
    gl::DeleteBuffers(1,&ebo_);gl::DeleteBuffers(1,&vbo_);gl::DeleteVertexArrays(1,&vao_);
}
void Mesh::draw() const {
    gl::BindVertexArray(vao_);gl::DrawElements(gl::TRIANGLES,count_,gl::UNSIGNED_INT,nullptr);
}
Mesh Mesh::cube() {
    std::vector<Vertex> vertices;std::vector<unsigned int> indices;
    const Vec3 normals[]={{0,0,1},{0,0,-1},{1,0,0},{-1,0,0},{0,1,0},{0,-1,0}};
    const Vec3 tangents[]={{1,0,0},{-1,0,0},{0,0,-1},{0,0,1},{1,0,0},{1,0,0}};
    for(int face=0;face<6;++face) {
        Vec3 n=normals[face],t=tangents[face],b=cross(n,t);
        unsigned int start=static_cast<unsigned int>(vertices.size());
        vertices.push_back({(n-t-b)*0.5f,n,0,0});
        vertices.push_back({(n+t-b)*0.5f,n,1,0});
        vertices.push_back({(n+t+b)*0.5f,n,1,1});
        vertices.push_back({(n-t+b)*0.5f,n,0,1});
        for(unsigned int i:{0u,1u,2u,0u,2u,3u}) indices.push_back(start+i);
    }
    return Mesh(vertices,indices);
}
}
