#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	vec3 ambient;
	float shininess;
};

struct Light {
	vec3 direction;
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float kConstant;
	float kLinear;
	float kQuadratic;

	float cutOff;
	float outerCutOff;
};

out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main()
{
	vec3 lightDir = normalize( light.position - FragPos );
	float theta = dot( lightDir, normalize(-light.direction) );
	if( theta > light.outerCutOff )
	{
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = (theta - light.outerCutOff) / epsilon;
	// Ambient
	vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

	// Diffuse
	vec3 norm = normalize( Normal );
	float diff= max( dot(lightDir, norm), 0.0f );
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

	// Specular
	vec3 viewDir = normalize( cameraPos - FragPos );
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow( max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

	// attenuation
	float distance = length( FragPos - light.position );
	float attenuation = 1.0f / ( light.kConstant + 
								 light.kLinear * distance +
								 light.kQuadratic * (distance * distance) );
	ambient *= attenuation;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	vec3 result = (ambient + diffuse + specular);
	color = vec4( result, 1.0f );
	}
	else
	{
		color = vec4( light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0f);
	}

}
