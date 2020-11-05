// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
// struct for material
#version 330 core
out vec4 Color;

in vec3 TexCoords;

uniform samplerCube Sphere;
uniform int debugColor;

void main()
{
    if(debugColor == 1)
    {
        Color = vec4(1.0,0.0,0.0,1.0);
    }
    else
    {
        Color = texture(Sphere, TexCoords);
    }
}
