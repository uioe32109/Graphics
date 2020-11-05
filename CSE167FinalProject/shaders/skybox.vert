#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

//layout (location = 0) in vec3 aPos;
//
//out vec3 TexCoords;
//out vec3 cloudPos;
//uniform mat4 projection;
//uniform mat4 view;
////out vec3 frag;
//
//void main()
//{
//    TexCoords = aPos;
//    gl_Position = projection * view * vec4(aPos, 1.0);
//}

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
uniform mat4 projection;
uniform mat4 view;
//out vec3 frag;
//const vec2 data[4] = vec2[](
//  vec2(-1.0,  1.0), vec2(-1.0, -1.0),
//  vec2( 1.0,  1.0), vec2( 1.0, -1.0));




void main()
{
//    gl_Position = vec4(data[gl_VertexID], 0.0, 1.0);
//    pos = transpose(mat3(view)) * (inverse(projection) * gl_Position).xyz;
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
    //fsun = vec3(0.0, 0.0, 0.0);
}



    

