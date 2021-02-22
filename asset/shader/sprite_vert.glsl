#version 330 core

layout(location = 0) in vec2 v_Position;
layout(location = 1) in vec2 v_UV;

out vec2 UV;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(v_Position, 0.0, 1.0);
    UV = v_UV;
}