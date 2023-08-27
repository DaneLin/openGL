#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
//添加的扰动
const float offset = 1.0f / 300.0f;

void main()
{
    FragColor = texture(screenTexture, TexCoords);
    //反相的处理效果
    //FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

    //灰度效果
    // FragColor = texture(screenTexture, TexCoords);
    // float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    // FragColor = vec4(average, average, average, 1.0);

    //采用加权方式的灰度效果
    // FragColor = texture(screenTexture, TexCoords);
    // float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    // FragColor = vec4(average, average, average, 1.0);

//     vec2 offsets[9] = vec2[](
//         vec2(-offset, offset),
//         vec2(0.0f, offset),
//         vec2(offset, offset),
//         vec2(-offset, 0.0f),
//         vec2(0.0f, 0.0f),
//         vec2(offset, 0.0f),
//         vec2(-offset, -offset),
//         vec2(0.0f, -offset),
//         vec2(offset, -offset)
//     );

//     float kernel[9] = float[] (
//         -1, -1, -1,
//         -1, 9, -1,
//         -1, -1, -1
//     );
// //     float kernel[9] = float[](
// //     1.0 / 16, 2.0 / 16, 1.0 / 16,
// //     2.0 / 16, 4.0 / 16, 2.0 / 16,
// //     1.0 / 16, 2.0 / 16, 1.0 / 16  
// // );
//     // float kernel[9] = float[](
//     //     1, 1, 1,
//     //     1, -8, 1,
//     //     1, 1, 1
//     // );

//     vec3 sampleTex[9];
//     for (int i = 0; i < 9; i++)
//     {
//         sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
//     }
//     vec3 col = vec3(0);
//     for (int i = 0; i < 9; i++)
//         col += sampleTex[i] * kernel[i];

//     FragColor = vec4(col, 1.0f);

}