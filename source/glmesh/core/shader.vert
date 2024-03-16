#version 400 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout( location = 0 ) in vec4 vPosition;

void main()
{
    gl_Position = projection * view * model * vPosition;
}