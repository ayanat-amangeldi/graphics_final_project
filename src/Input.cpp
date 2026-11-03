#include "concert/Input.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace concert {
Input::Input(GLFWwindow* window,bool captureMouse):window_(window) {
    glfwSetWindowUserPointer(window_,this);
    glfwSetKeyCallback(window_,keyCallback);glfwSetCursorPosCallback(window_,cursorCallback);
    glfwSetWindowFocusCallback(window_,focusCallback);
    if(captureMouse) toggleCursor();
}
Input::~Input() {
    glfwSetKeyCallback(window_,nullptr);glfwSetCursorPosCallback(window_,nullptr);
    glfwSetWindowFocusCallback(window_,nullptr);glfwSetWindowUserPointer(window_,nullptr);
    glfwSetInputMode(window_,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
}
void Input::poll() {
    pressed_.fill(false);mouseDX=mouseDY=0;glfwPollEvents();
}
bool Input::down(int key) const {
    return key>=0 && key<static_cast<int>(held_.size()) && held_[key];
}
bool Input::pressed(int key) const {
    return key>=0 && key<static_cast<int>(pressed_.size()) && pressed_[key];
}
void Input::toggleCursor() {
    captured_=!captured_;
    glfwSetInputMode(window_,GLFW_CURSOR,captured_?GLFW_CURSOR_DISABLED:GLFW_CURSOR_NORMAL);
    resetMouse();
}
void Input::resetMouse() { firstMouse_=true;mouseDX=mouseDY=0; }
void Input::keyCallback(GLFWwindow* window,int key,int,int action,int) {
    auto& self=*static_cast<Input*>(glfwGetWindowUserPointer(window));
    if(key<0 || key>=static_cast<int>(self.held_.size())) return;
    if(action==GLFW_PRESS) {self.held_[key]=true;self.pressed_[key]=true;}
    if(action==GLFW_RELEASE) self.held_[key]=false;
}
void Input::cursorCallback(GLFWwindow* window,double x,double y) {
    auto& self=*static_cast<Input*>(glfwGetWindowUserPointer(window));
    if(!self.captured_) return;
    if(self.firstMouse_) {self.lastX_=x;self.lastY_=y;self.firstMouse_=false;return;}
    self.mouseDX+=x-self.lastX_;self.mouseDY+=y-self.lastY_;
    self.lastX_=x;self.lastY_=y;
}
void Input::focusCallback(GLFWwindow* window,int focused) {
    auto& self=*static_cast<Input*>(glfwGetWindowUserPointer(window));
    self.resetMouse();
    if(!focused) {
        self.held_.fill(false);self.pressed_.fill(false);
        if(self.captured_) self.toggleCursor();
    }
}
}
