#pragma once

namespace ShaderSource {
    const char* kVertexShaderSource = R"(#version 400 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout( location = 0 ) in vec4 vPosition;

void main()
{
    gl_Position = projection * view * model * vPosition;
})";
    const char* kFragmentShaderSource = R"(#version 450 core

out vec4 fColor;

void main()
{
    fColor = vec4(0.5, 0.4, 0.8, 1.0);
})";
}
