#version 330 core

out vec4 color;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 modelColor;

uniform vec3 lightPos;

void main()
{
	// ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize( Normal );
	vec3 lightDir = normalize( lightPos - FragPos );
	float diffuse = max( dot(norm, lightDir), 0.0 );
	vec3 diffuseColor = diffuse * lightColor;

	vec3 result = ( ambient + diffuse )* modelColor;
	color = vec4( result, 1.0f );
}
