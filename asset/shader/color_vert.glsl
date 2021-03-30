#version 300 es

layout(location = 0) in vec2  a_Position;
layout(location = 1) in vec2  a_UV;
layout(location = 2) in vec4  a_Color;
layout(location = 3) in float a_Texture;

out vec2  v_UV;
out vec4  v_Color;
out float v_Texure;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 0.0, 1.0);
    v_UV = a_UV;
    v_Color = a_Color;
    v_Texure = a_Texture;
}