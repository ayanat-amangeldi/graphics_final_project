#pragma once
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace concert {
constexpr float pi = 3.14159265358979323846f;
constexpr float radians(float degrees) { return degrees * pi / 180.0f; }
struct Vec3 { float x = 0, y = 0, z = 0; };
inline Vec3 operator+(Vec3 a, Vec3 b) { return {a.x+b.x,a.y+b.y,a.z+b.z}; }
inline Vec3 operator-(Vec3 a, Vec3 b) { return {a.x-b.x,a.y-b.y,a.z-b.z}; }
inline Vec3 operator*(Vec3 a, float s) { return {a.x*s,a.y*s,a.z*s}; }
inline Vec3 operator/(Vec3 a, float s) { return a*(1.0f/s); }
inline float dot(Vec3 a, Vec3 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
inline Vec3 cross(Vec3 a, Vec3 b) {
    return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};
}
inline float length(Vec3 a) { return std::sqrt(dot(a,a)); }
inline Vec3 normalize(Vec3 a) { float n=length(a); return n>1e-7f ? a/n : Vec3{}; }

// Column-major storage, column vectors: world = parent * local * position.
struct Mat4 {
    float m[16]{};
    static Mat4 identity() { Mat4 r; r.m[0]=r.m[5]=r.m[10]=r.m[15]=1; return r; }
};
inline Mat4 operator*(const Mat4& a, const Mat4& b) {
    Mat4 r;
    for(int c=0;c<4;++c) for(int row=0;row<4;++row)
        for(int k=0;k<4;++k) r.m[c*4+row]+=a.m[k*4+row]*b.m[c*4+k];
    return r;
}
inline Vec3 transformPoint(const Mat4& m, Vec3 p) {
    float w=m.m[3]*p.x+m.m[7]*p.y+m.m[11]*p.z+m.m[15];
    if(std::abs(w)<1e-7f) throw std::runtime_error("Cannot project a point with w=0");
    return {(m.m[0]*p.x+m.m[4]*p.y+m.m[8]*p.z+m.m[12])/w,
            (m.m[1]*p.x+m.m[5]*p.y+m.m[9]*p.z+m.m[13])/w,
            (m.m[2]*p.x+m.m[6]*p.y+m.m[10]*p.z+m.m[14])/w};
}
inline Mat4 translation(Vec3 p) {
    Mat4 r=Mat4::identity(); r.m[12]=p.x;r.m[13]=p.y;r.m[14]=p.z;return r;
}
inline Mat4 scaling(Vec3 p) {
    Mat4 r=Mat4::identity();r.m[0]=p.x;r.m[5]=p.y;r.m[10]=p.z;return r;
}
inline Mat4 rotationY(float a) {
    Mat4 r=Mat4::identity();float c=std::cos(a),s=std::sin(a);
    r.m[0]=c;r.m[8]=s;r.m[2]=-s;r.m[10]=c;return r;
}
inline Mat4 rotationX(float a) {
    Mat4 r=Mat4::identity();float c=std::cos(a),s=std::sin(a);
    r.m[5]=c;r.m[9]=-s;r.m[6]=s;r.m[10]=c;return r;
}
inline Mat4 lookAt(Vec3 eye, Vec3 target, Vec3 up={0,1,0}) {
    Vec3 f=normalize(target-eye),s=normalize(cross(f,up)),u=cross(s,f);
    Mat4 r=Mat4::identity();
    r.m[0]=s.x;r.m[4]=s.y;r.m[8]=s.z;r.m[12]=-dot(s,eye);
    r.m[1]=u.x;r.m[5]=u.y;r.m[9]=u.z;r.m[13]=-dot(u,eye);
    r.m[2]=-f.x;r.m[6]=-f.y;r.m[10]=-f.z;r.m[14]=dot(f,eye);return r;
}
inline Mat4 perspective(float fov, float aspect, float nearPlane, float farPlane) {
    if(aspect<=0 || nearPlane<=0 || farPlane<=nearPlane || fov<=0 || fov>=pi)
        throw std::runtime_error("Invalid perspective parameters");
    Mat4 r;float f=1/std::tan(fov/2);
    r.m[0]=f/aspect;r.m[5]=f;r.m[10]=(farPlane+nearPlane)/(nearPlane-farPlane);
    r.m[11]=-1;r.m[14]=2*farPlane*nearPlane/(nearPlane-farPlane);return r;
}
}
