
#version 330 core

in vec2 vUV;
in vec3 vNormal;

out vec4 FragColor;

uniform float uTime;
uniform int uPreset;

void main()
{
    if (vNormal.z < 0.5) {
        FragColor = vec4(0.015, 0.02, 0.04, 1.0);
        return;
    }

    vec2 uv = vUV;

    float wave = sin(
        uv.x * 18.0 +
        uTime * 2.0 +
        sin(uv.y * 10.0 - uTime)
    );

    float pattern = 0.5 + 0.5 * wave;

    vec3 colorA = vec3(0.05, 0.7, 1.0);
    vec3 colorB = vec3(0.85, 0.1, 1.0);

    if (uPreset == 2) {
        colorA = vec3(1.0, 0.1, 0.5);
        colorB = vec3(0.0, 0.9, 1.0);
    }

    if (uPreset == 3) {
        colorA = vec3(1.0, 0.35, 0.05);
        colorB = vec3(1.0, 0.85, 0.2);
    }

    vec3 color = mix(colorA, colorB, pattern);

    float glow = 0.75 + 0.25 * sin(uTime * 1.5);
    color *= glow;

    FragColor = vec4(color, 1.0);
}
