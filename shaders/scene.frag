#version 330 core
in vec3 vWorldPosition;
in vec3 vNormal;
in vec2 vUV;
out vec4 fragColor;
uniform vec3 uColor;
uniform vec3 uCamera;
uniform vec3 uLightPosition0;
uniform vec3 uLightPosition1;
uniform vec3 uLightColor0;
uniform vec3 uLightColor1;
uniform int uGrid;
uniform float uEmission;

vec3 lightContribution(vec3 position,vec3 color,vec3 albedo,vec3 n,vec3 viewDir) {
    vec3 delta=position-vWorldPosition;
    float distanceToLight=length(delta);
    vec3 lightDir=delta/max(distanceToLight,0.0001);
    float diffuse=max(dot(n,lightDir),0.0);
    vec3 halfway=normalize(lightDir+viewDir);
    float specular=diffuse>0.0 ? pow(max(dot(n,halfway),0.0),32.0) : 0.0;
    float attenuation=1.0/(1.0+0.025*distanceToLight+0.005*distanceToLight*distanceToLight);
    return (albedo*diffuse+vec3(0.22)*specular)*color*attenuation;
}
void main() {
    vec3 albedo=uColor;
    if(uGrid!=0) {
        // One-metre world-space grid, with derivative-based antialiasing.
        vec2 uv=vWorldPosition.xz;
        vec2 distanceToLine=abs(fract(uv-0.5)-0.5);
        vec2 pixelWidth=max(fwidth(uv),vec2(0.0001));
        float line=1.0-min(min(distanceToLine.x/pixelWidth.x,distanceToLine.y/pixelWidth.y),1.0);
        albedo=mix(albedo,vec3(0.19,0.27,0.34),line*0.75);
    }
    vec3 n=normalize(vNormal),viewDir=normalize(uCamera-vWorldPosition);
    vec3 result=albedo*0.18;
    result+=lightContribution(uLightPosition0,uLightColor0,albedo,n,viewDir);
    result+=lightContribution(uLightPosition1,uLightColor1,albedo,n,viewDir);
    result+=albedo*uEmission;
    fragColor=vec4(pow(max(result,vec3(0.0)),vec3(1.0/2.2)),1.0);
}
