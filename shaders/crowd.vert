#version 330 core
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aUV;
layout(location=3) in vec4 aInstanceModel0;
layout(location=4) in vec4 aInstanceModel1;
layout(location=5) in vec4 aInstanceModel2;
layout(location=6) in vec4 aInstanceModel3;

uniform mat4 uView;
uniform mat4 uProjection;
uniform float uAnimationTime;
layout(location=7) in vec4 aInstanceStyle;

flat out vec3 vClothingColor;
flat out int vPart;

void main() {
    mat4 model = mat4(aInstanceModel0, aInstanceModel1,
                      aInstanceModel2, aInstanceModel3);
    vec3 position = aPosition;
    vPart = int(aUV.x + 0.5);
    float phase = aInstanceStyle.w;
    float idle = sin(uAnimationTime * 1.15 + phase);

    // Arm geometry is tagged 1 (left) and 2 (right). Rotate around each
    // shoulder so waving happens independently within the shared mesh.
    if (vPart == 1 || vPart == 2) {
        bool selectedArm = (vPart == 1 && gl_InstanceID % 5 == 0) ||
                           (vPart == 2 && gl_InstanceID % 5 == 2);
        float pulse = max(0.0, sin(uAnimationTime * 0.7 + phase));
        float wave = selectedArm ? smoothstep(0.78, 0.98, pulse) * 2.0 : 0.0;
        float angle = (vPart == 1 ? -1.0 : 1.0) * (wave + 0.025 * idle);
        vec2 pivot = vec2(vPart == 1 ? -0.22 : 0.22, 1.15);
        vec2 arm = position.xy - pivot;
        position.xy = pivot + mat2(cos(angle), sin(angle), -sin(angle), cos(angle)) * arm;
    }

    // A small per-person lean creates a gentle side-to-side sway.
    float sway = 0.035 * idle;
    vec2 body = position.xy - vec2(0.0, 0.75);
    position.xy = vec2(0.0, 0.75) + mat2(cos(sway), sin(sway), -sin(sway), cos(sway)) * body;

    vClothingColor = aInstanceStyle.rgb;
    gl_Position = uProjection * uView * model * vec4(position, 1.0);
}
