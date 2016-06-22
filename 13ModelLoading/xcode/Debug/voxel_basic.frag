#version 330 core
struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
out vec4 color;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform DirLight dirLight;

void main()
{
	// Ambient
	vec3 ambient = vec3( texture(texture_diffuse1, TexCoords) ) * dirLight.ambient;

	// Diffuse
	vec3 norm = normalize( Normal );
	vec3 lightDir = normalize( -dirLight.direction );
	float diff = max( dot(norm, lightDir), 0.0f );
	vec3 diffuse = diff * vec3(texture(texture_diffuse1, TexCoords)) * dirLight.diffuse;

	vec3 result = ( diffuse + ambient );
	color = vec4( result, 1.0f );

}
