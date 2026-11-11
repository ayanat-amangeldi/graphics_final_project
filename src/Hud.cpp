#include "concert/Hud.hpp"
#include <algorithm>
#include <map>
#include <sstream>

namespace concert {
namespace {
// Original 5x7 bitmap alphabet, rendered as indexed triangles in the core pipeline.
const std::map<char,const char*> glyphs={
 {'A',"01110/10001/10001/11111/10001/10001/10001"},
 {'B',"11110/10001/10001/11110/10001/10001/11110"},
 {'C',"01111/10000/10000/10000/10000/10000/01111"},
 {'D',"11110/10001/10001/10001/10001/10001/11110"},
 {'E',"11111/10000/10000/11110/10000/10000/11111"},
 {'F',"11111/10000/10000/11110/10000/10000/10000"},
 {'G',"01111/10000/10000/10111/10001/10001/01111"},
 {'H',"10001/10001/10001/11111/10001/10001/10001"},
 {'I',"11111/00100/00100/00100/00100/00100/11111"},
 {'J',"00111/00010/00010/00010/10010/10010/01100"},
 {'K',"10001/10010/10100/11000/10100/10010/10001"},
 {'L',"10000/10000/10000/10000/10000/10000/11111"},
 {'M',"10001/11011/10101/10101/10001/10001/10001"},
 {'N',"10001/11001/10101/10011/10001/10001/10001"},
 {'O',"01110/10001/10001/10001/10001/10001/01110"},
 {'P',"11110/10001/10001/11110/10000/10000/10000"},
 {'Q',"01110/10001/10001/10001/10101/10010/01101"},
 {'R',"11110/10001/10001/11110/10100/10010/10001"},
 {'S',"01111/10000/10000/01110/00001/00001/11110"},
 {'T',"11111/00100/00100/00100/00100/00100/00100"},
 {'U',"10001/10001/10001/10001/10001/10001/01110"},
 {'V',"10001/10001/10001/10001/10001/01010/00100"},
 {'W',"10001/10001/10001/10101/10101/10101/01010"},
 {'X',"10001/10001/01010/00100/01010/10001/10001"},
 {'Y',"10001/10001/01010/00100/00100/00100/00100"},
 {'Z',"11111/00001/00010/00100/01000/10000/11111"},
 {'0',"01110/10001/10011/10101/11001/10001/01110"},
 {'1',"00100/01100/00100/00100/00100/00100/01110"},
 {'2',"01110/10001/00001/00010/00100/01000/11111"},
 {'3',"11110/00001/00001/01110/00001/00001/11110"},
 {'4',"00010/00110/01010/10010/11111/00010/00010"},
 {'5',"11111/10000/10000/11110/00001/00001/11110"},
 {'6',"01110/10000/10000/11110/10001/10001/01110"},
 {'7',"11111/00001/00010/00100/01000/01000/01000"},
 {'8',"01110/10001/10001/01110/10001/10001/01110"},
 {'9',"01110/10001/10001/01111/00001/00001/01110"},
 {'-',"00000/00000/00000/11111/00000/00000/00000"},
 {':',"00000/00100/00100/00000/00100/00100/00000"},
 {'.',"00000/00000/00000/00000/00000/00100/00100"},
 {'/',"00001/00010/00010/00100/01000/01000/10000"},
 {'[',"01110/01000/01000/01000/01000/01000/01110"},
 {']',"01110/00010/00010/00010/00010/00010/01110"}
};
void quad(std::vector<Vertex>& v,std::vector<unsigned int>& i,float x,float y,float w,float h) {
    auto start=static_cast<unsigned int>(v.size());
    v.push_back({{x,y,0},{0,0,1},0,0});v.push_back({{x+w,y,0},{0,0,1},0,0});
    v.push_back({{x+w,y+h,0},{0,0,1},0,0});v.push_back({{x,y+h,0},{0,0,1},0,0});
    for(unsigned int offset:{0u,1u,2u,0u,2u,3u}) i.push_back(start+offset);
}
}
void Hud::draw(int width,int height,float fps,const SceneState& state) {
    
    std::ostringstream out;

    out << "CONCERT STAGE"
        << "\nFPS: " << static_cast<int>(fps)
        << "  PRESET: " << state.preset
        << "  " << (state.paused ? "PAUSED" : "RUNNING");

    if (state.showHelp) {
        out << "\nWASD MOVE  MOUSE LOOK  Q/E UP-DOWN"
            << "\nSHIFT FAST  TAB CURSOR  R RESET"
            << "\n1/2/3 PRESETS"
            << "\nL LED: "
            << (state.ledAnimated ? "ANIMATING" : "FROZEN")
            << "\nP PAUSE  H HELP  ESC EXIT"
            << "\nSPACE FOCUS REQUEST"
            << "\nC CONFETTI REQUEST";
}

    std::string text=out.str();
    if(text!=lastText_ || width!=lastWidth_ || height!=lastHeight_) {
        lastText_=text;lastWidth_=width;lastHeight_=height;
        float scale=std::max(0.5f,std::min(static_cast<float>(width)/1280,
                                        static_cast<float>(height)/720));
        float pixel=1.65f*scale,x=18*scale,y=18*scale,maxX=x;
        std::vector<Vertex> vertices;std::vector<unsigned int> indices;
        for(char c:text) {
            if(c=='\n') {maxX=std::max(maxX,x);x=18*scale;y+=12*pixel;continue;}
            auto it=glyphs.find(c);
            if(it!=glyphs.end()) {
                const char* glyph=it->second;
                for(int row=0;row<7;++row) for(int col=0;col<5;++col)
                    if(glyph[row*6+col]=='1') quad(vertices,indices,x+col*pixel,y+row*pixel,pixel,pixel);
            }
            x+=6*pixel;
        }
        maxX=std::max(maxX,x);
        text_=std::make_unique<Mesh>(vertices,indices);
        vertices.clear();indices.clear();
        quad(vertices,indices,10*scale,10*scale,maxX,y+9*pixel-10*scale);
        background_=std::make_unique<Mesh>(vertices,indices);
    }
    gl::Disable(gl::DEPTH_TEST);gl::Disable(gl::CULL_FACE);
    shader_.use();shader_.set("uScreen",Vec3{static_cast<float>(width),static_cast<float>(height),0});
    shader_.set("uColor",Vec3{0.025f,0.035f,0.055f});background_->draw();
    shader_.set("uColor",Vec3{0.78f,0.88f,0.95f});text_->draw();
    gl::Enable(gl::DEPTH_TEST);gl::Enable(gl::CULL_FACE);
}
}
