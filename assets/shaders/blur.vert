#version 430 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

layout (location = 0) out vec2 outUV;

void main()
{
    outUV = uv;
    gl_Position =  vec4(pos, 0.0, 1.0);
}