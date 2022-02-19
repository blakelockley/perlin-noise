#version 330 core

out vec4 FragColor;

in vec2 Texcoord;

uniform sampler2D ourTexture;

void main()
{
    vec4 color = texture(ourTexture, Texcoord);
    FragColor = vec4(color.r, color.r, color.r, 1.0);
}