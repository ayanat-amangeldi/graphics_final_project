#pragma once
#include "Config.hpp"

namespace concert {
struct SingerPose {
    Mat4 root;
    Mat4 upperBody;
    float gaitPhase, stride;

    Vec3 focusPoint() const {
        return transformPoint(upperBody, config::singerTarget - config::singerFeet);
    }
};

inline SingerPose singerPose(double time) {
    const double travel = std::fmod(time, 8.0) * (2.0 * pi / 8.0);
    const float x = 2.0f * static_cast<float>(std::sin(travel));
    const float direction = static_cast<float>(std::cos(travel));
    const float stride = std::abs(direction);
    const float yaw = radians(69) * std::tanh(4.0f * direction);
    const float gait = static_cast<float>(std::fmod(time, 0.8) * (2.0 * pi / 0.8));
    const float bob = 0.025f * stride * std::cos(2 * gait);
    const Mat4 root = translation(config::singerFeet + Vec3{x, 0, 0}) * rotationY(yaw);
    return {root, root * translation({0, bob, 0}), gait, stride};
}
}
