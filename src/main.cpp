#include "concert/Camera.hpp"
#include "concert/GL.hpp"
#include "concert/Hud.hpp"
#include "concert/Scene.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using namespace concert;
struct Options {bool smoke=false;std::filesystem::path shaderDir,screenshot;};
Options options(int argc,char** argv) {
    Options r;
    for(int i=1;i<argc;++i) {
        std::string a=argv[i];
        if(a=="--smoke-test") r.smoke=true;
        else if((a=="--shader-dir" || a=="--screenshot") && i+1<argc) {
            if(a=="--shader-dir") r.shaderDir=argv[++i];else r.screenshot=argv[++i];
        } else throw std::runtime_error("Usage: concert_stage [--smoke-test] [--shader-dir PATH] [--screenshot FILE.ppm]");
    }
    if(!r.screenshot.empty() && !r.smoke)
        throw std::runtime_error("--screenshot requires --smoke-test");
    if(!r.shaderDir.empty()) return r;
    std::vector<std::filesystem::path> roots;
    // Prefer shaders beside the executable, then the project working directory.
    roots.push_back(std::filesystem::absolute(argv[0]).parent_path()/"shaders");
    roots.push_back(std::filesystem::current_path()/"shaders");
#ifdef CONCERT_SOURCE_DIR
    roots.push_back(std::filesystem::path(CONCERT_SOURCE_DIR)/"shaders");
#endif
    for(const auto& p:roots) if(std::filesystem::exists(p/"scene.vert")) {r.shaderDir=p;return r;}
    throw std::runtime_error("Shaders not found. Run from the repository root or use --shader-dir PATH.");
}
void checkGL() {
    auto error=gl::GetError();
    if(error!=gl::NO_ERROR) throw std::runtime_error("OpenGL error: "+std::to_string(error));
}
void saveScreenshot(const std::filesystem::path& path,int width,int height) {
    std::vector<unsigned char> pixels(static_cast<std::size_t>(width)*height*3);
    gl::ReadBuffer(gl::BACK);gl::PixelStorei(gl::PACK_ALIGNMENT,1);
    gl::ReadPixels(0,0,width,height,gl::RGB,gl::UNSIGNED_BYTE,pixels.data());checkGL();
    std::ofstream file(path,std::ios::binary);
    if(!file) throw std::runtime_error("Cannot write screenshot: "+path.string());
    file<<"P6\n"<<width<<' '<<height<<"\n255\n";
    for(int row=height-1;row>=0;--row)
        file.write(reinterpret_cast<const char*>(pixels.data()+static_cast<std::size_t>(row)*width*3),width*3);
    if(!file) throw std::runtime_error("Screenshot write failed");
}
void controls(Input& input,SceneState& state,Camera& camera,GLFWwindow* window) {
    if(input.pressed(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window,GLFW_TRUE);
    if(input.pressed(GLFW_KEY_TAB)) input.toggleCursor();
    if(input.pressed(GLFW_KEY_R)) {camera.reset();input.resetMouse();}
    for(int i=1;i<=3;++i) if(input.pressed(GLFW_KEY_1+i-1)) state.preset=i;
    if(input.pressed(GLFW_KEY_P)) state.paused=!state.paused;
    if(input.pressed(GLFW_KEY_H)) state.showHelp=!state.showHelp;
    if(input.pressed(GLFW_KEY_L)) state.ledAnimated=!state.ledAnimated;
    if(input.pressed(GLFW_KEY_C)) ++state.confettiRequest;
    state.focusSinger=input.down(GLFW_KEY_SPACE);
}
void run(GLFWwindow* window,const Options& opt) {
    gl::load();
    std::cout<<"OpenGL: "<<gl::GetString(gl::VERSION)<<"\nGPU: "<<gl::GetString(gl::RENDERER)
             <<"\nShaders: "<<opt.shaderDir.string()<<"\n";
    Input input(window,!opt.smoke);Camera camera;Scene scene;SceneState state;
    Renderer renderer(opt.shaderDir);Hud hud(opt.shaderDir);
    double previous=glfwGetTime(),fpsStart=previous,runStart=previous;int fpsFrames=0,frames=0;
    float fps=0;
    std::cout<<"TAB releases mouse. H toggles controls. ESC quits.\n";
    while(!glfwWindowShouldClose(window)) {
        input.poll();controls(input,state,camera,window);
        double now=glfwGetTime();
        // Clamp long stalls so restoring a window does not jump the camera/particles.
        float dt=static_cast<float>(std::clamp(now-previous,0.0,0.1));previous=now;
        camera.update(input,dt);
        float animationDT=state.paused?0:dt;
        state.animationTime+=animationDT;scene.update(state,animationDT);
        int width=0,height=0;glfwGetFramebufferSize(window,&width,&height);
        if(width==0 || height==0) {
            if(opt.smoke) throw std::runtime_error("Smoke test needs a visible non-minimized framebuffer");
            glfwWaitEventsTimeout(0.05);continue;
        }
        renderer.begin(camera,width,height);scene.draw(renderer,state);
        ++fpsFrames;
        if(now-fpsStart>=0.5) {
            fps=static_cast<float>(fpsFrames/(now-fpsStart));fpsFrames=0;fpsStart=now;
            std::string title="Concert Stage | "+std::to_string(static_cast<int>(fps))+" FPS | Scaffold";
            glfwSetWindowTitle(window,title.c_str());
        }
        hud.draw(width,height,fps,state);
        bool smokeDone=opt.smoke && frames>=119 && now-runStart>=1.0;
        if(opt.smoke) {
            checkGL();
            if(smokeDone && !opt.screenshot.empty()) saveScreenshot(opt.screenshot,width,height);
        }
        glfwSwapBuffers(window);++frames;
        if(smokeDone) glfwSetWindowShouldClose(window,GLFW_TRUE);
    }
    if(opt.smoke) std::cout<<"SMOKE PASS: "<<frames<<" frames; shaders linked; no OpenGL errors.\n";
    // Local GPU resources are destroyed before returning to main's context cleanup.
}
}
int main(int argc,char** argv) {
    GLFWwindow* window=nullptr;bool initialized=false;
    try {
        Options opt=options(argc,argv);
        glfwSetErrorCallback([](int code,const char* text){std::cerr<<"GLFW "<<code<<": "<<text<<'\n';});
        if(!glfwInit()) throw std::runtime_error("GLFW initialization failed");
        initialized=true;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GLFW_TRUE);
        // Use real 1280x720 framebuffer pixels on Retina for the initial performance check.
#ifdef __APPLE__
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER,GLFW_FALSE);
#endif
        window=glfwCreateWindow(config::windowWidth,config::windowHeight,"Concert Stage | Scaffold",nullptr,nullptr);
        if(!window) throw std::runtime_error("Cannot create an OpenGL 3.3 core window");
        glfwMakeContextCurrent(window);glfwSwapInterval(1);
        run(window,opt);
        glfwDestroyWindow(window);glfwTerminate();return 0;
    } catch(const std::exception& e) {
        std::cerr<<"ERROR: "<<e.what()<<'\n';
        if(window) glfwDestroyWindow(window);
        if(initialized) glfwTerminate();return 1;
    }
}
