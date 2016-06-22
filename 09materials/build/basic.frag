#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 color;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main()
{
	// Ambient
	vec3 ambient = material.ambient * light.ambient;

	// Diffuse
	vec3 norm = normalize( Normal );
	vec3 lightDir = normalize( light.position - FragPos );
	float diff= max( dot(lightDir, norm), 0.0f );
	vec3 diffuse = material.diffuse * diff * light.diffuse;

	// Specular
	vec3 viewDir = normalize( cameraPos - FragPos );
	vec3 reflectDir = normalize( reflect(-lightDir, norm) );
	float spec = pow( max(dot(viewDir, reflectDir), 0.0), material.shininess * 50);
	//float spec = pow( max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = spec * light.specular * material.specular;

	vec3 result = ambient + diffuse + specular;
	color = vec4( result, 1.0f );

}
