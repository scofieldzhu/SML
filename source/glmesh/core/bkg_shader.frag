#version 450 core

out vec4 frag_color;

in vec3 out_color; 

void main()
{
    frag_color = vec4(out_color, 1.0);
}
