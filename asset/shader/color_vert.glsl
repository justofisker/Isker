#version 330 core

layout(location = 0) in vec3 v_Position;

void main()
{
    gl_Position = v_Position;
}