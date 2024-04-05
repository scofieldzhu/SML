#version 400 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec4 vertex_clr;
layout(location = 2) in vec3 vertex_normal;
uniform vec4 user_color;
uniform int use_vcolor;
uniform vec3 user_normal;
uniform int use_vnormal;

const int PRIMITIVE_TYPE_SPHERE = 0;
const int PRIMITIVE_TYPE_CLOUD_MESH = 1;
uniform int primitive_type;

out vec4 out_color;

void main()
{
    if(primitive_type == PRIMITIVE_TYPE_SPHERE){
        gl_Position = projection * view * vec4(vertex_pos, 1.0);
    }else if(primitive_type == PRIMITIVE_TYPE_CLOUD_MESH){
        gl_Position = projection * view * model * vec4(vertex_pos, 1.0);
    }
    if(use_vcolor == 1){
        out_color = vertex_clr;
    }else{
        out_color = user_color;
    }        
}