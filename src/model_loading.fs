#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

//光照
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

uniform sampler2D texture_diffuse1;

void main()
{    
    vec3 ambient = vec3(1.0f, 1.0f, 1.0f) * light.ambient;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = vec3(1.0f, 1.0f, 1.0f) * diff * light.diffuse;

    //specular
    vec3 viewDir = normalize(light.position - FragPos);
    vec3 reflectDir = reflect(-viewDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64.f);
    vec3 specular = vec3(1.0f, 1.0f, 1.0f) * spec * light.specular;

    vec3 result = ambient + specular + diffuse;
    FragColor = vec4(result, 1.0f);
}