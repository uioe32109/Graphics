#version 330 core
// This is a sample fragment shader.
in float sampleExtraOutput;

uniform vec3 color;
in vec3 new_normal;
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(new_normal, sampleExtraOutput);
}
