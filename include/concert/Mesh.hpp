#pragma once
#include "GL.hpp"
#include "Math.hpp"
#include <vector>

namespace concert {
struct Vertex { Vec3 position,normal;float u=0,v=0; };
class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indices);
    ~Mesh();
    Mesh(const Mesh&)=delete;
    Mesh& operator=(const Mesh&)=delete;
    void draw() const;
    static Mesh cube(); // Unit cube centred at the local origin; full face normals and UVs.
private:
    gl::UInt vao_=0,vbo_=0,ebo_=0;
    gl::Size count_=0;
};
}
