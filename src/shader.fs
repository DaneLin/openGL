#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position; //使用定向光就不再需要位置向量了
    //vec3 direction;//使用光的方向向量
    vec3 direction;
    float cutOff;
    float outCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //实现衰减
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;
uniform Material material;

void main()
{
    
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outCutOff;
    float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0f, 1.0f);

   
        //执行光照计算
        //ambient
    // vec3 ambient = light.ambient *  material.ambient;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)).rgb;

    //diffuse
    vec3 norm = normalize(Normal);
    
    //vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    //emission
    vec3 emission = vec3(texture(material.emission, TexCoords));

    //attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    //vec3 result = specular * objectColor;
    diffuse *= intensity;
    specular *= intensity;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}