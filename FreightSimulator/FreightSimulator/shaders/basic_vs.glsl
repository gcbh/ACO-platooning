#version 330
// Inputs per vertex
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;

// Output data will be interpolated for each fragment.
out vec2 UV;

// Uniforms throughout the mesh
uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1.0);

    // UV of the vertex. No special space for this one.
    UV = vertexUV;
}
