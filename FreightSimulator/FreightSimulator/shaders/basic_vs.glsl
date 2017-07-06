#version 400
// Inputs per vertex
in vec3 vp;

// Uniforms throughout the mesh
uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vp, 1.0);
    //gl_Position = vec4(vp, 1.0);
}
