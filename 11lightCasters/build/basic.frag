#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform float lightIntensity;


uniform Material material;
uniform Light light;



void main()
{
	// ambient
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * vec3(texture(material.diffuse, TexCoords))  * light.ambient;


	// diffuse
	vec3 norm = normalize( Normal );
	vec3 lightDir = normalize( light.position - FragPos );
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff * light.diffuse;

	// specular
	float specularStrength = 1.0f;
	vec3 viewDir = normalize( cameraPos - FragPos );
	vec3 reflectDir = reflect( -lightDir, norm );
	float spec = pow(max(dot(viewDir,reflectDir), 0.0), 256);
	vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

	// result
	vec3 result = (ambient + diffuse + specular);
	//color = vec4( 1.0f, 0.5f, 0.2f, 1.0f );
	color = vec4(result, 1.0f) * lightIntensity;

}
