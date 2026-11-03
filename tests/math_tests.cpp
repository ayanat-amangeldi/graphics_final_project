#include "concert/Config.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

using namespace concert;
namespace {
void near(float actual,float expected,const char* label) {
    if(std::abs(actual-expected)>0.0001f) {
        std::cerr<<"FAIL "<<label<<": "<<actual<<" != "<<expected<<'\n';std::exit(1);
    }
}
void near(Vec3 a,Vec3 b,const char* label) {near(a.x,b.x,label);near(a.y,b.y,label);near(a.z,b.z,label);}
}
int main() {
    // Parent translation and rotation must also rotate the child's local offset.
    Mat4 parent=translation({3,2,1})*rotationY(radians(90));
    Mat4 child=translation({2,0,0})*scaling({2,3,4});
    near(transformPoint(parent*child,{0,0,0}),{3,2,-1},"parent-child pivot");
    near(transformPoint(parent*child,{0.5f,0,0}),{3,2,-2},"local scale then rotation");
    Mat4 view=lookAt({0,3,16},{0,2.5f,0});
    near(transformPoint(view,{0,3,16}),{0,0,0},"camera eye maps to origin");
    Vec3 target=transformPoint(view,{0,2.5f,0});
    near(target.x,0,"view target x");near(target.y,0,"view target y");
    if(target.z>=0) return 1;
    Mat4 projection=perspective(radians(60),16.0f/9,0.1f,100);
    near(transformPoint(projection,{0,0,-0.1f}).z,-1,"near plane NDC");
    near(transformPoint(projection,{0,0,-100}).z,1,"far plane NDC");
    near(cross({1,0,0},{0,1,0}),{0,0,1},"right handed coordinates");
    near(config::singerFeet.y,config::stageCenter.y+config::stageSize.y/2,"singer on stage");
    for(Vec3 anchor:config::rigAnchors) {
        if(std::abs(anchor.x)>config::stageSize.x/2 || std::abs(anchor.z)>config::stageSize.z/2)
            return 1;
    }
    bool rejected=false;
    try {perspective(radians(60),0,0.1f,100);} catch(const std::runtime_error&) {rejected=true;}
    if(!rejected) return 1;
    std::cout<<"PASS: hierarchy, camera view, projection, handedness and shared coordinates.\n";
}
