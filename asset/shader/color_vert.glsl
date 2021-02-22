#version 330 core

layout(location = 0) in vec2 v_Position;
layout(location = 1) in vec2 v_UV;
layout(location = 2) in vec4 v_Color;
layout(location = 3) in float v_Texture;

out vec2 UV;
out vec4 Color;
out float Tex;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(v_Position, 0.0, 1.0);
    UV = v_UV;
    Color = v_Color;
    Tex = v_Texture;
}