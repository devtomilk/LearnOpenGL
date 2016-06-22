#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 M = mat4( 1.0f );
uniform mat4 V = mat4( 1.0f );
uniform mat4 P = mat4( 1.0f );

out vec3 FragPos; // world coordinate
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	gl_Position = P * V * M * vec4( position, 1.0f );
	Normal = normal;
	FragPos = vec3( M * vec4(position, 1.0f) );
	TexCoords = texCoords;
}
