#include "concert/Camera.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace concert {
void Camera::reset() {
    position_=config::cameraStart;
    Vec3 f=normalize(config::cameraTarget-position_);
    yaw_=std::atan2(f.z,f.x);pitch_=std::asin(f.y);
}
Vec3 Camera::forward() const {
    return {std::cos(yaw_)*std::cos(pitch_),std::sin(pitch_),std::sin(yaw_)*std::cos(pitch_)};
}
void Camera::update(const Input& input,float dt) {
    // Mouse displacement already measures movement; do not multiply it by dt.
    yaw_+=static_cast<float>(input.mouseDX)*0.0025f;
    pitch_=std::clamp(pitch_-static_cast<float>(input.mouseDY)*0.0025f,radians(-89),radians(89));
    Vec3 f=forward(),right=normalize(cross(f,{0,1,0})),movement{};
    if(input.down(GLFW_KEY_W)) movement=movement+f;
    if(input.down(GLFW_KEY_S)) movement=movement-f;
    if(input.down(GLFW_KEY_D)) movement=movement+right;
    if(input.down(GLFW_KEY_A)) movement=movement-right;
    if(input.down(GLFW_KEY_E)) movement.y+=1;
    if(input.down(GLFW_KEY_Q)) movement.y-=1;
    float speed=(input.down(GLFW_KEY_LEFT_SHIFT)||input.down(GLFW_KEY_RIGHT_SHIFT))?12.0f:4.0f;
    position_=position_+normalize(movement)*(speed*dt);
}
}
