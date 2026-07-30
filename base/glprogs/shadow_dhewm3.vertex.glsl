#version 100
precision mediump float;

attribute vec4 attr_Vertex;

uniform mat4 u_modelViewProjectionMatrix;
uniform vec4 u_lightOrigin;

void main(void)
{
    gl_Position = u_modelViewProjectionMatrix * (attr_Vertex.w * u_lightOrigin + attr_Vertex - u_lightOrigin);
}
