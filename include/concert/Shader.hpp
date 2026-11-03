#pragma once
#include "GL.hpp"
#include "Math.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>

namespace concert {
class Shader {
public:
    Shader(const std::filesystem::path& vertex,const std::filesystem::path& fragment);
    ~Shader();
    Shader(const Shader&)=delete;
    Shader& operator=(const Shader&)=delete;
    void use() const;
    void set(const std::string& name,const Mat4& value);
    void set(const std::string& name,Vec3 value);
    void set(const std::string& name,float value);
    void set(const std::string& name,int value);
private:
    gl::UInt program_=0;
    std::unordered_map<std::string,gl::Int> locations_;
    gl::Int location(const std::string& name);
};
}
