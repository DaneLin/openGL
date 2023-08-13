#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
uniform float horizontal_offset;

void main()
{
    gl_Position = vec4(aPos.x + horizontal_offset, aPos.y, aPos.z, 1.0);
    ourColor = vec3(aColor.x + horizontal_offset, aColor.y, aColor.z + horizontal_offset);
}