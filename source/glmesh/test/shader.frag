#version 140
#extension GL_ARB_explicit_attrib_location : require

out vec4 FragColor;
//layout (location = 0) out vec4 fragColor;

in vec4 color;

void main()
{
    FragColor = color;
    //fragColor = vec4(fragColor.x, 0.0, fragColor.y, 1.0);
}