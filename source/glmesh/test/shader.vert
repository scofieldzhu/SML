#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec3 vertex;

out vec4 color;

uniform mat4 project_mat;
uniform mat4 view_mat;
uniform mat4 model_mat;

void main()
{
    //gl_Position = model_mat * view_mat * project_mat * vec4(vertex, 1.0);
    gl_Position = vec4(vertex, 1.0);
    color = vec4(1.0, 0.0, 0.0, 1.0);
}