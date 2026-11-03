#pragma once
#include "Config.hpp"
#include "Input.hpp"

namespace concert {
class Camera {
public:
    Camera() {reset();}
    void reset();
    void update(const Input& input,float dt);
    Mat4 view() const {return lookAt(position_,position_+forward());}
    Vec3 position() const {return position_;}
private:
    Vec3 position_{};
    float yaw_=0,pitch_=0;
    Vec3 forward() const;
};
}
