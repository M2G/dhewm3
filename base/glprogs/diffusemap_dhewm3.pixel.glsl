#version 100
precision mediump float;

varying vec2 var_TexCoord;
varying lowp vec4 var_Color;

uniform sampler2D u_fragmentMap0;
uniform lowp vec4 u_glColor;

void main(void)
{
    gl_FragColor = texture2D(u_fragmentMap0, var_TexCoord) * u_glColor * var_Color;
}
