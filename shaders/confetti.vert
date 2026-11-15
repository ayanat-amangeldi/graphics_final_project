#version 330 core
layout(location=0) in vec2 aCorner;
layout(location=1) in vec3 aInstancePosition;
layout(location=2) in vec3 aInstanceColor;
layout(location=3) in vec2 aInstanceSize;
layout(location=4) in float aInstanceRotation;

uniform mat4 uView;
uniform mat4 uProjection;
out vec3 vColor;

void main() {
    float c=cos(aInstanceRotation),s=sin(aInstanceRotation);
    mat2 rotation=mat2(c,s,-s,c);
    vec2 local=rotation*(aCorner*aInstanceSize);
    vec3 world=aInstancePosition+vec3(local,0.0);
    vColor=aInstanceColor;
    gl_Position=uProjection*uView*vec4(world,1.0);
}
