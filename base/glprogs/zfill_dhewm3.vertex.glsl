#version 100
precision mediump float;

attribute highp vec4 attr_Vertex;
attribute vec4 attr_TexCoord;

uniform highp mat4 u_modelViewProjectionMatrix;
uniform mat4 u_textureMatrix;

varying vec2 var_TexDiffuse;

void main(void)
{
    var_TexDiffuse = (u_textureMatrix * attr_TexCoord).xy;
    gl_Position = u_modelViewProjectionMatrix * attr_Vertex;
}
