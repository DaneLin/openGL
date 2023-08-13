#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec3 ourColor;
out vec2 TexCoord;
uniform float horizontal_offset;

void main()
{
    gl_Position = vec4(aPos.x + horizontal_offset, aPos.y, aPos.z, 1.0);
    ourColor = vec3(aColor.x + horizontal_offset, aColor.y, aColor.z + horizontal_offset);
    TexCoord = aTexCoords;
}