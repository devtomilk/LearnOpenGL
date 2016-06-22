#version 330 core

out vec4 color;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
	// Ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	vec3 norm = normalize( Normal );
	vec3 lightDir = normalize( lightPos - FragPos );
	float diff= max( dot(lightDir, norm), 0.0f );
	vec3 diffuse = diff * lightColor;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize( cameraPos - FragPos );
	vec3 reflectDir = normalize( reflect(-lightDir, norm) );
	float spec = pow( max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = spec * specularStrength * lightColor;

	color = vec4( (ambient + diffuse + specular) * objectColor, 1.0f );
	//color = vec4( objectColor * lightColor, 1.0f );
}
