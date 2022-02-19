#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

out vec2 Texcoord;

uniform mat4 view, projection;

void main()
{
    gl_Position = vec4(position, 0.0f, 1.0f);
    Texcoord = texcoord;
}
