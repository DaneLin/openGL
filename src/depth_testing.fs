#version 330 core
out vec4 FragColor;

void main()
{
    // float ratio = 1.00 / 1.52;
    // vec3 I = normalize(Position - cameraPos);
    // vec3 R = refract(I, normalize(Normal),ratio);
    // FragColor = vec4(texture(skybox, R).rgb, 1.0);

    // if (gl_FragCoord.x < 400)
    // {
    //     FragColor = vec4(1.0, 0.0, 0.0, 1.0f);
    // }
    // else
    // {
    //     FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    // }
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}