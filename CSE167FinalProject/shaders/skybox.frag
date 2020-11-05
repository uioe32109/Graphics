// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
// struct for material
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube Skybox;
//uniform sampler2D Cloud;

void main()
{
    FragColor = texture(Skybox, TexCoords);
}


