#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 M = mat4(1.0);
uniform mat4 V = mat4(1.0);
uniform mat4 P = mat4(1.0);

void main()
{
	gl_Position = P * V * M * vec4( position.x, position.y, position.z, 1.0 );
	Normal = normal;
	FragPos = vec3(M * vec4(position, 0.0f) );
}
