#version 330 core
out vec4 FragColor;

in vec3 fragmentPosition;
in vec3 normal;
in vec2 textureCoordinates;

uniform sampler2D diffuse;
uniform sampler2D specular;

uniform float shininess = 24;

struct Light 
{
    vec3 position;
    vec3 color;
    
    float constant;
    float linear;
    float quadratic;
};

uniform Light lights[8];
uniform int lightCount;
uniform vec3 viewPos;

vec3 calculateLight(Light light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 plainTextureColor = vec3(texture(diffuse, textureCoordinates));

    if (lightCount < 1)
    {
        vec3 ambientCoefficient = vec3(0.2, 0.2, 0.2);
        FragColor = vec4(ambientCoefficient * plainTextureColor, 1.0);
        return;
    }

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragmentPosition);
    
    vec3 result = vec3(0.0);

    for (int i = 0; i < lightCount; i++) 
    {
        result += calculateLight(lights[i], norm, viewDir);
    }
    
    FragColor = vec4(result, 1.0);
}

vec3 calculateLight(Light light, vec3 normal, vec3 viewDir)
{   
    vec3 lightDir = normalize(light.position - fragmentPosition);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
 
    vec3 diffuseLightIntensity = vec3(0.5, 0.5, 0.5);
    vec3 ambientLightIntensity = vec3(0.2, 0.2, 0.2) * diffuseLightIntensity;
    
    vec3 ambientLightColor = ambientLightIntensity * light.color;
    vec3 diffuseLightColor = diffuseLightIntensity * light.color;

    vec3 ambient = light.color * vec3(texture(diffuse, textureCoordinates));
    vec3 diffuse = light.color * diff * vec3(texture(diffuse, textureCoordinates));
    vec3 specular = light.color * spec * vec3(texture(specular, textureCoordinates));
    
    return (ambient + diffuse + specular) * attenuation;
}