#version 330 core
in vec3 vWorldPosition;
in vec3 vNormal;
in vec2 vUV;
out vec4 fragColor;
uniform vec3 uColor;
uniform vec3 uCamera;
struct Spotlight {
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 params; // outer cosine, inner cosine, intensity
};
uniform Spotlight uSpotlights[6];
uniform int uGrid;
uniform float uEmission;

vec3 lightContribution(Spotlight light,vec3 albedo,vec3 n,vec3 viewDir) {
    vec3 delta=light.position-vWorldPosition;
    float distanceToLight=length(delta);
    vec3 lightDir=delta/max(distanceToLight,0.0001);
    float cone = smoothstep(light.params.x, light.params.y, dot(-lightDir, light.direction));
    if (cone <= 0.0) return vec3(0.0);
    float diffuse=max(dot(n,lightDir),0.0);
    vec3 halfway=lightDir+viewDir;
    halfway /= max(length(halfway), 0.0001);
    float specular=diffuse>0.0 ? pow(max(dot(n,halfway),0.0),32.0) : 0.0;
    float attenuation=1.0/(1.0+0.025*distanceToLight+0.005*distanceToLight*distanceToLight);
    return (albedo*diffuse+vec3(0.22)*specular)*light.color*attenuation*cone*light.params.z;
}
void main() {
    vec3 albedo=uColor;
    if(uGrid!=0) {
        // One-metre world-space grid, with derivative-based antialiasing.
        vec2 uv=vWorldPosition.xz;
        vec2 distanceToLine=abs(fract(uv-0.5)-0.5);
        vec2 pixelWidth=max(fwidth(uv),vec2(0.0001));
        float line=1.0-min(min(distanceToLine.x/pixelWidth.x,distanceToLine.y/pixelWidth.y),1.0);
        albedo=mix(albedo,vec3(0.10,0.13,0.20),line*0.26);
    }
    vec3 n=normalize(vNormal),viewDir=normalize(uCamera-vWorldPosition);
    // A soft wash keeps silhouettes legible between moving spotlights.
    float fill = max(dot(n, normalize(vec3(0.15, 0.48, 1.0))), 0.0);
    vec3 result = albedo * (vec3(0.13, 0.15, 0.22) + vec3(0.35, 0.38, 0.49) * fill);
    for (int i = 0; i < 6; ++i) {
        result += lightContribution(uSpotlights[i], albedo, n, viewDir);
    }
    result+=albedo*uEmission;
    fragColor=vec4(pow(max(result,vec3(0.0)),vec3(1.0/2.2)),1.0);
}
