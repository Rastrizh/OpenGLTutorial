#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse[2];
    sampler2D specular[2];
	sampler2D normal[2];
	sampler2D hight[2];
    float shininess;
};   
uniform Material material;

struct DirectionLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
 uniform DirectionLight directionlight;

 uniform vec3 cameraPos;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // обратно к NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

 vec3 CalculateDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir);

 void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(cameraPos - FragPos);

	vec3 result = CalculateDirectionLight(directionlight, norm, viewDir);
	
	float depth = LinearizeDepth(gl_FragCoord.z) / far; // делим на far для наглядности
    FragColor = vec4(vec3(depth), 1.0);

    //FragColor = vec4(result, 1.0);
}

vec3 CalculateDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	for(int i = 0; i < 2; i++)
	{
		ambient = light.ambient * vec3(texture(material.diffuse[i], TexCoords));
		diffuse = light.diffuse * diff * vec3(texture(material.diffuse[i], TexCoords));
		specular = light.specular * spec * vec3(texture(material.specular[i], TexCoords));
	}
	return (ambient + diffuse + specular);
}