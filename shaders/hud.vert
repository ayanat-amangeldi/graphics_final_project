#version 330 core
layout(location=0) in vec3 aPosition;
uniform vec3 uScreen;
void main() {
    gl_Position=vec4(aPosition.x/uScreen.x*2.0-1.0,1.0-aPosition.y/uScreen.y*2.0,0.0,1.0);
}
