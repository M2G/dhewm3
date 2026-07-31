#version 100
precision mediump float;

attribute highp vec4 attr_Vertex;
attribute lowp vec4 attr_Color;
attribute vec4 attr_TexCoord;

uniform highp mat4 u_modelViewProjectionMatrix;
uniform mat4 u_textureMatrix;
uniform lowp float u_colorAdd;
uniform lowp float u_colorModulate;

varying vec2 var_TexCoord;
varying lowp vec4 var_Color;

void main(void)
{
    var_TexCoord = (u_textureMatrix * attr_TexCoord).xy;

    if (u_colorModulate == 0.0) {
        var_Color = vec4(u_colorAdd);
    } else {
        var_Color = (attr_Color * u_colorModulate) + vec4(u_colorAdd);
    }

    gl_Position = u_modelViewProjectionMatrix * attr_Vertex;
}
