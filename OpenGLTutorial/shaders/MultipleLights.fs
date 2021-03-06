#version 450 core
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
  
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
 uniform DirectionLight directionlight;

 struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  
 
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight {
    vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
SpotLight spotlight;

uniform vec3 cameraPos;

vec4 CalculateDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
 
void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(cameraPos - FragPos);

	vec4 result = CalculateDirectionLight(directionlight, norm, viewDir);

	//for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //    result += CalculatePointLight(pointLights[i], norm, FragPos, viewDir);

	//result += CalculateSpotLight(spotlight, norm, FragPos, viewDir);
	
    FragColor = result;
}

vec4 CalculateDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	for(int i = 0; i < 2; i++)
	{
		ambient  += texture(material.diffuse[i], TexCoords);
		if(ambient.a < 0.1)
			discard;
		diffuse  += texture(material.diffuse[i], TexCoords);
		specular += texture(material.specular[i], TexCoords);
	}
	ambient  *= light.ambient;
	diffuse  *= diff * light.diffuse;
	specular *= spec * light.specular;
	return (ambient + diffuse + specular);
}

//vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//	vec3 lightDir = normalize(light.position - fragPos);
//
//	float diff = max(dot(normal, lightDir), 0.0);
//
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//	float distance = length(light.position - fragPos);
//	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//
//	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//
//	ambient *= attenuation;
//	diffuse *= attenuation;
//	specular *= attenuation;
//
//	return (ambient + diffuse + specular);
//}
//
//vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//	vec3 lightDir = normalize(light.position - fragPos);
//
//	// ????????? ?????????
//	float diff = max(dot(normal, lightDir), 0.0);
//
//	// ?????????? ?????????
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//	// ?????????
//	float distance = length(light.position - fragPos);
//	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//
//	// ????????????? ??????????
//	float theta = dot(lightDir, normalize(-light.direction));
//	float epsilon = light.cutOff - light.outerCutOff;
//	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//
//	// ????????? ??????????
//	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//	ambient *= attenuation * intensity;
//	diffuse *= attenuation * intensity;
//	specular *= attenuation * intensity;
//	return (ambient + diffuse + specular);
//}