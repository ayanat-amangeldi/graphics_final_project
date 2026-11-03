#pragma once
// Small project-owned OpenGL 3.3 loader. GLFW supplies the platform addresses.
// No platform GL headers, GLAD download or generated third-party code needed.
#include <cstddef>
#if defined(_WIN32)
#define CONCERT_GL_CALL __stdcall
#else
#define CONCERT_GL_CALL
#endif

namespace concert::gl {
using Enum=unsigned int;using UInt=unsigned int;using Int=int;using Size=int;
using Bool=unsigned char;using Bits=unsigned int;using Float=float;
using SizePtr=std::ptrdiff_t;using Char=char;
constexpr Enum ARRAY_BUFFER=0x8892,ELEMENT_ARRAY_BUFFER=0x8893,STATIC_DRAW=0x88E4;
constexpr Enum FLOAT=0x1406,UNSIGNED_INT=0x1405,UNSIGNED_BYTE=0x1401;
constexpr Enum TRIANGLES=0x0004,VERTEX_SHADER=0x8B31,FRAGMENT_SHADER=0x8B30;
constexpr Enum COMPILE_STATUS=0x8B81,LINK_STATUS=0x8B82,INFO_LOG_LENGTH=0x8B84;
constexpr Enum DEPTH_TEST=0x0B71,CULL_FACE=0x0B44,BLEND=0x0BE2;
constexpr Enum COLOR_BUFFER_BIT=0x4000,DEPTH_BUFFER_BIT=0x0100;
constexpr Enum VERSION=0x1F02,RENDERER=0x1F01,RGB=0x1907,PACK_ALIGNMENT=0x0D05;
constexpr Enum NO_ERROR=0,BACK=0x0405;
constexpr Bool FALSE_VALUE=0;

#define CONCERT_GL_FUNCTIONS(X) \
 X(void,Viewport,(Int,Int,Size,Size)) \
 X(void,ClearColor,(Float,Float,Float,Float)) \
 X(void,Clear,(Bits)) \
 X(void,Enable,(Enum)) \
 X(void,Disable,(Enum)) \
 X(Enum,GetError,()) \
 X(const unsigned char*,GetString,(Enum)) \
 X(void,GenVertexArrays,(Size,UInt*)) \
 X(void,DeleteVertexArrays,(Size,const UInt*)) \
 X(void,BindVertexArray,(UInt)) \
 X(void,GenBuffers,(Size,UInt*)) \
 X(void,DeleteBuffers,(Size,const UInt*)) \
 X(void,BindBuffer,(Enum,UInt)) \
 X(void,BufferData,(Enum,SizePtr,const void*,Enum)) \
 X(void,EnableVertexAttribArray,(UInt)) \
 X(void,VertexAttribPointer,(UInt,Int,Enum,Bool,Size,const void*)) \
 X(UInt,CreateShader,(Enum)) \
 X(void,ShaderSource,(UInt,Size,const Char* const*,const Int*)) \
 X(void,CompileShader,(UInt)) \
 X(void,GetShaderiv,(UInt,Enum,Int*)) \
 X(void,GetShaderInfoLog,(UInt,Size,Size*,Char*)) \
 X(void,DeleteShader,(UInt)) \
 X(UInt,CreateProgram,()) \
 X(void,AttachShader,(UInt,UInt)) \
 X(void,LinkProgram,(UInt)) \
 X(void,GetProgramiv,(UInt,Enum,Int*)) \
 X(void,GetProgramInfoLog,(UInt,Size,Size*,Char*)) \
 X(void,DeleteProgram,(UInt)) \
 X(void,UseProgram,(UInt)) \
 X(Int,GetUniformLocation,(UInt,const Char*)) \
 X(void,UniformMatrix4fv,(Int,Size,Bool,const Float*)) \
 X(void,Uniform3f,(Int,Float,Float,Float)) \
 X(void,Uniform1f,(Int,Float)) \
 X(void,Uniform1i,(Int,Int)) \
 X(void,DrawElements,(Enum,Size,Enum,const void*)) \
 X(void,ReadPixels,(Int,Int,Size,Size,Enum,Enum,void*)) \
 X(void,ReadBuffer,(Enum)) \
 X(void,PixelStorei,(Enum,Int))
#define CONCERT_GL_DECLARE(result,name,args) \
 using name##Function=result (CONCERT_GL_CALL*) args; extern name##Function name;
CONCERT_GL_FUNCTIONS(CONCERT_GL_DECLARE)
#undef CONCERT_GL_DECLARE
void load();
}
