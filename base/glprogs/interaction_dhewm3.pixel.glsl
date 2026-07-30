#version 100
precision mediump float;

varying vec3 v_texcoord0_lightDirTS;
varying vec2 v_texcoord1_bump;
varying float v_texcoord2_falloff;
varying vec3 v_texcoord3_projLight;
varying vec2 v_texcoord4_diffuse;
varying vec2 v_texcoord5_specular;
varying vec3 v_texcoord6_halfAngleTS;
varying vec4 v_color;

uniform sampler2D u_bumpMap;
uniform sampler2D u_falloffMap;
uniform sampler2D u_projectionMap;
uniform sampler2D u_diffuseMap;
uniform sampler2D u_specularMap;
uniform vec4 u_diffuseModifier;
uniform vec4 u_specularModifier;

void main() {
    vec3 L = normalize(v_texcoord0_lightDirTS);
    vec3 H = normalize(v_texcoord6_halfAngleTS);
    vec3 N = normalize(2.0 * texture2D(u_bumpMap, v_texcoord1_bump).agb - 1.0);

    float NdotL = clamp(dot(N, L), 0.0, 1.0);
    float NdotH = clamp(dot(N, H), 0.0, 1.0);

    vec3 lightProjection = texture2DProj(u_projectionMap, v_texcoord3_projLight).rgb;
    vec3 lightFalloff = texture2D(u_falloffMap, vec2(v_texcoord2_falloff, 0.5)).rgb;
    vec3 diffuseColor = texture2D(u_diffuseMap, v_texcoord4_diffuse).rgb * u_diffuseModifier.rgb;
    vec3 specularColor = 2.0 * texture2D(u_specularMap, v_texcoord5_specular).rgb * u_specularModifier.rgb;

    float specularFalloff = pow(NdotH, 12.0);

    vec3 color = diffuseColor;
    color += specularFalloff * specularColor;
    color *= NdotL * lightProjection;
    color *= lightFalloff;

    gl_FragColor = vec4(color, 1.0) * v_color;
}
