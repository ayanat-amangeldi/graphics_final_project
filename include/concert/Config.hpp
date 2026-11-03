#pragma once
#include "Math.hpp"
#include <array>

namespace concert::config {
constexpr int windowWidth=1280, windowHeight=720;
constexpr Vec3 stageSize{16,1,8}, stageCenter{0,0.5f,0};
constexpr Vec3 singerFeet{0,1,2}, singerTarget{0,2.4f,2};
constexpr Vec3 cameraStart{0,3,16}, cameraTarget{0,2.5f,0};
constexpr Vec3 ledCenter{0,4,-3.8f}, ledSize{10,4,0.12f};
constexpr std::array<Vec3,6> rigAnchors{{{-6,7,-3},{0,7,-3},{6,7,-3},
                                         {-6,7,3},{0,7,3},{6,7,3}}};
constexpr std::array<Vec3,2> confettiOrigins{{{-6,5,2},{6,5,2}}};
constexpr int crowdColumns=12, crowdRows=10;
constexpr float crowdMinX=-10, crowdMaxX=10, crowdMinZ=7, crowdMaxZ=22;
constexpr float fov=radians(60), nearPlane=0.1f, farPlane=100;
}
