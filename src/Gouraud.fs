#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;

in vec3 LightColor;

void main()
{
    FragColor = vec4(LightColor * objectColor, 1.0f);
}