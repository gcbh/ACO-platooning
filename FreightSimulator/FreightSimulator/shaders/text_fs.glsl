#version 330

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

    // Output color = color
    color.rgb = vec3(1.0, 1.0, 1.0);

    //Convert to greyscale
    float gray = dot(texture( myTextureSampler, UV ).rgb, vec3(0.299, 0.587, 0.114));
    color.a = gray;
}
