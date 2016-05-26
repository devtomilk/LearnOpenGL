#version 330 core

out vec4 color;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 cameraPos;

void main()
{
	// ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;


	// diffuse
	vec3 norm = normalize( Normal );
	vec3 lightDir = normalize( lightPos - FragPos );
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 1.0f;
	vec3 viewDir = normalize( cameraPos - FragPos );
	vec3 reflectDir = reflect( -lightDir, norm );
	float spec = pow(max(dot(viewDir,reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	// result
	vec3 result = (ambient + diffuse + specular) * objectColor;
	//color = vec4( 1.0f, 0.5f, 0.2f, 1.0f );
	color = vec4( result, 1.0f );
}
