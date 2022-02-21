#version 330 core

out vec4 FragColor;

in vec2 Texcoord;

uniform sampler2D ourTexture;

void main()
{
    float t = texture(ourTexture, Texcoord).r;
    FragColor = vec4(mix(vec3(0, 0, 1), vec3(0, 1, 0), t), 1.0f);
}