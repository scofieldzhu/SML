#version 400 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec4 vertex_pos;
layout(location = 1) in vec4 vertex_clr;
layout(location = 2) in vec3 vertex_normal;
uniform vec4 user_color;
uniform int use_vcolor;
uniform vec3 user_normal;
uniform int use_vnormal;
out vec4 out_color;

void main()
{
    gl_Position = projection * view * model * vertex_pos;
    if(use_vcolor == 1){
        out_color = vertex_clr;
    }else{
        out_color = user_color;
    }        
}