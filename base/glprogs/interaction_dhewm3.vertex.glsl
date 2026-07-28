#version 120

uniform mat4 u_modelViewProj;

uniform vec4 u_localLightOrigin;
uniform vec4 u_localViewOrigin;
uniform vec4 u_lightProjectionS;
uniform vec4 u_lightProjectionT;
uniform vec4 u_lightProjectionQ;
uniform vec4 u_lightFalloffS;
uniform vec4 u_bumpMatrixS;
uniform vec4 u_bumpMatrixT;
uniform vec4 u_diffuseMatrixS;
uniform vec4 u_diffuseMatrixT;
uniform vec4 u_specularMatrixS;
uniform vec4 u_specularMatrixT;
uniform vec4 u_colorModulate;
uniform vec4 u_colorAdd;

attribute vec3 in_position;
attribute vec4 in_color;
attribute vec2 in_texcoord;
attribute vec3 in_normal;
attribute vec3 in_tangent0;
attribute vec3 in_tangent1;

varying vec3 v_texcoord0_lightDirTS;
varying vec2 v_texcoord1_bump;
varying float v_texcoord2_falloff;
varying vec3 v_texcoord3_projLight;
varying vec2 v_texcoord4_diffuse;
varying vec2 v_texcoord5_specular;
varying vec3 v_texcoord6_halfAngleTS;
varying vec4 v_color;

void main() {
    vec4 worldPos = vec4(in_position, 1.0);
    gl_Position = u_modelViewProj * worldPos;

    vec3 toLight = u_localLightOrigin.xyz - in_position;
    v_texcoord0_lightDirTS = vec3(
        dot(in_tangent0, toLight),
        dot(in_tangent1, toLight),
        dot(in_normal,   toLight)
    );

    v_texcoord1_bump = vec2(
        dot(vec4(in_texcoord, 0.0, 1.0), u_bumpMatrixS),
        dot(vec4(in_texcoord, 0.0, 1.0), u_bumpMatrixT)
    );

    v_texcoord2_falloff = dot(worldPos, u_lightFalloffS);

    v_texcoord3_projLight = vec3(
        dot(worldPos, u_lightProjectionS),
        dot(worldPos, u_lightProjectionT),
        dot(worldPos, u_lightProjectionQ)
    );

    v_texcoord4_diffuse = vec2(
        dot(vec4(in_texcoord, 0.0, 1.0), u_diffuseMatrixS),
        dot(vec4(in_texcoord, 0.0, 1.0), u_diffuseMatrixT)
    );

    v_texcoord5_specular = vec2(
        dot(vec4(in_texcoord, 0.0, 1.0), u_specularMatrixS),
        dot(vec4(in_texcoord, 0.0, 1.0), u_specularMatrixT)
    );

    vec3 normToLight = normalize(toLight);
    vec3 toView = u_localViewOrigin.xyz - in_position;
    vec3 normToView = normalize(toView);
    vec3 halfAngle = normToLight + normToView;

    v_texcoord6_halfAngleTS = vec3(
        dot(in_tangent0, halfAngle),
        dot(in_tangent1, halfAngle),
        dot(in_normal,   halfAngle)
    );

    v_color = in_color * u_colorModulate + u_colorAdd;
}
