#include "concert/Shader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace concert {
namespace {
gl::UInt compile(gl::Enum type,const std::filesystem::path& path) {
    std::ifstream file(path);
    if(!file) throw std::runtime_error("Cannot open shader: "+path.string());
    std::stringstream buffer;buffer<<file.rdbuf();std::string code=buffer.str();
    const char* source=code.c_str();gl::UInt id=gl::CreateShader(type);
    gl::ShaderSource(id,1,&source,nullptr);gl::CompileShader(id);
    gl::Int ok=0;gl::GetShaderiv(id,gl::COMPILE_STATUS,&ok);
    if(!ok) {
        gl::Int n=0;gl::GetShaderiv(id,gl::INFO_LOG_LENGTH,&n);
        std::vector<char> log(static_cast<std::size_t>(n>0?n:1));
        gl::GetShaderInfoLog(id,static_cast<gl::Size>(log.size()),nullptr,log.data());
        gl::DeleteShader(id);
        throw std::runtime_error("Shader compilation failed: "+path.string()+"\n"+log.data());
    }
    return id;
}
}
Shader::Shader(const std::filesystem::path& vertex,const std::filesystem::path& fragment) {
    gl::UInt vs=compile(gl::VERTEX_SHADER,vertex),fs=0;
    try { fs=compile(gl::FRAGMENT_SHADER,fragment); }
    catch(...) { gl::DeleteShader(vs);throw; }
    program_=gl::CreateProgram();gl::AttachShader(program_,vs);gl::AttachShader(program_,fs);
    gl::LinkProgram(program_);gl::DeleteShader(vs);gl::DeleteShader(fs);
    gl::Int ok=0;gl::GetProgramiv(program_,gl::LINK_STATUS,&ok);
    if(!ok) {
        gl::Int n=0;gl::GetProgramiv(program_,gl::INFO_LOG_LENGTH,&n);
        std::vector<char> log(static_cast<std::size_t>(n>0?n:1));
        gl::GetProgramInfoLog(program_,static_cast<gl::Size>(log.size()),nullptr,log.data());
        gl::DeleteProgram(program_);program_=0;
        throw std::runtime_error(std::string("Shader link failed:\n")+log.data());
    }
}
Shader::~Shader() { if(program_) gl::DeleteProgram(program_); }
void Shader::use() const { gl::UseProgram(program_); }
gl::Int Shader::location(const std::string& name) {
    auto it=locations_.find(name);if(it!=locations_.end()) return it->second;
    return locations_.emplace(name,gl::GetUniformLocation(program_,name.c_str())).first->second;
}
void Shader::set(const std::string& name,const Mat4& v) {
    gl::UniformMatrix4fv(location(name),1,gl::FALSE_VALUE,v.m);
}
void Shader::set(const std::string& name,Vec3 v) { gl::Uniform3f(location(name),v.x,v.y,v.z); }
void Shader::set(const std::string& name,float v) { gl::Uniform1f(location(name),v); }
void Shader::set(const std::string& name,int v) { gl::Uniform1i(location(name),v); }
}
