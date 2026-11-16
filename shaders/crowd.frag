#version 330 core
flat in vec3 vClothingColor;
flat in int vPart;
in vec3 vNormal;
out vec4 fragColor;

void main() {
    vec3 color = vClothingColor;
    if (vPart == 3) color = vec3(0.78, 0.57, 0.42); // simple skin tone
    else if (vPart == 4) color *= 0.62;              // darker trousers
    vec3 normal = normalize(vNormal);
    float wash = max(dot(normal, normalize(vec3(0.3, 0.7, -1.0))), 0.0);
    float overhead = max(normal.y, 0.0);
    color *= 0.42 + 0.30 * wash + 0.20 * overhead;
    color += vec3(0.05, 0.055, 0.10) * (0.65 + 0.35 * overhead);
    fragColor = vec4(color, 1.0);
}
