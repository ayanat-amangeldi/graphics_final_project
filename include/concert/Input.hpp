#pragma once
#include <array>
struct GLFWwindow;

namespace concert {
// Owns all GLFW callbacks. Scene modules must not install callbacks themselves.
class Input {
public:
    explicit Input(GLFWwindow* window,bool captureMouse);
    ~Input();
    void poll();
    bool down(int key) const;
    bool pressed(int key) const;
    void toggleCursor();
    void resetMouse();
    double mouseDX=0,mouseDY=0;
private:
    GLFWwindow* window_;
    std::array<bool,512> held_{},pressed_{};
    bool captured_=false,firstMouse_=true;
    double lastX_=0,lastY_=0;
    static void keyCallback(GLFWwindow*,int,int,int,int);
    static void cursorCallback(GLFWwindow*,double,double);
    static void focusCallback(GLFWwindow*,int);
};
}
