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

uniform vec3 cameraPos;
 
void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-directionlight.direction);
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
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
	ambient  *= directionlight.ambient;
	diffuse  *= diff * directionlight.diffuse;
	specular *= spec * directionlight.specular;
    
    FragColor = vec4(ambient + diffuse + specular);
}