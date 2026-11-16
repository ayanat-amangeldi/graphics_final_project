#version 330 core
in vec3 vWorldPosition;
in vec3 vNormal;
in vec2 vUV;
out vec4 fragColor;

uniform vec3 uColor;
uniform vec3 uCamera;
uniform float uOpacity;

void main() {
    vec3 towardCamera = uCamera - vWorldPosition;
    towardCamera /= max(length(towardCamera), 0.0001);
    float facing = abs(dot(normalize(vNormal), towardCamera));
    float opacity = uOpacity * smoothstep(0.0, 0.55, facing)
                    * smoothstep(0.0, 0.04, vUV.y)
                    * (1.0 - smoothstep(0.73, 1.0, vUV.y));
    fragColor = vec4(sqrt(uColor), opacity);
}
