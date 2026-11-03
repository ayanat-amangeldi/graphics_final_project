#include "concert/GL.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

namespace concert::gl {
#define CONCERT_GL_DEFINE(result,name,args) name##Function name=nullptr;
CONCERT_GL_FUNCTIONS(CONCERT_GL_DEFINE)
#undef CONCERT_GL_DEFINE
void load() {
#define CONCERT_GL_LOAD(result,name,args) \
 name=reinterpret_cast<name##Function>(glfwGetProcAddress("gl" #name)); \
 if(!name) throw std::runtime_error("Missing OpenGL entry point: gl" #name);
 CONCERT_GL_FUNCTIONS(CONCERT_GL_LOAD)
#undef CONCERT_GL_LOAD
}
}
