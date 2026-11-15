#version 330 core
flat in vec3 vClothingColor;
flat in int vPart;
out vec4 fragColor;

void main() {
    vec3 color = vClothingColor;
    if (vPart == 3) color = vec3(0.78, 0.57, 0.42); // simple skin tone
    else if (vPart == 4) color *= 0.62;              // darker trousers
    fragColor = vec4(color, 1.0);
}
