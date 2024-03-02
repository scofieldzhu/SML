#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec4 vertex;

out vec4 color;

void main()
{
    gl_Position = vertex;
    color = vec4(1.0, 1.0, 0.0, 1.0);
}