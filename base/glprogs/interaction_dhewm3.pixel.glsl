#version 120

varying vec3 v_texcoord0_lightDirTS;
varying vec2 v_texcoord1_bump;
varying float v_texcoord2_falloff;
varying vec2 v_texcoord4_diffuse;
varying vec4 v_color;

uniform sampler2D u_bumpMap;
uniform sampler2D u_diffuseMap;
uniform sampler2D u_falloffMap;
uniform vec4 u_diffuseModifier;

void main() {
    vec3 lightDir = normalize(v_texcoord0_lightDirTS);
    vec4 rawNormal = texture2D(u_bumpMap, v_texcoord1_bump);
    vec3 localNormal = normalize(vec3(rawNormal.a, rawNormal.y, rawNormal.z) * 2.0 - 1.0);
    float diffuseTerm = max(dot(lightDir, localNormal), 0.0);

    float falloffSample = texture2D(u_falloffMap, vec2(v_texcoord2_falloff, 0.5)).r;
    diffuseTerm *= falloffSample;

    vec4 diffuseColor = texture2D(u_diffuseMap, v_texcoord4_diffuse) * u_diffuseModifier;
    vec3 color = diffuseColor.rgb * diffuseTerm;

    gl_FragColor = vec4(min(color, vec3(1.0)), 1.0) * v_color;
}
