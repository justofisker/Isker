#version 330 core

layout(location = 0) in vec2 v_Position;
layout(location = 1) in vec2 v_UV;

out vec2 UV;

void main()
{
    UV = v_UV;
    gl_Position = vec4(v_Position, 0.0, 1.0);
}