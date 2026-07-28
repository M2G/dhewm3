#version 120

attribute vec4 attr_Vertex;

uniform mat4 u_modelViewProjectionMatrix;
uniform vec4 u_lightOrigin;

void main(void)
{
    // formule confirmee par d3wasm (production) : extrusion vers l'infini
    // via le composant .w de attr_Vertex (0 = extrude a l'infini, 1 = position reelle)
    gl_Position = u_modelViewProjectionMatrix * (attr_Vertex.w * u_lightOrigin + attr_Vertex - u_lightOrigin);
}
