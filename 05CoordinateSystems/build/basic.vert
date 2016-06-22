#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec3 OurColor;
out vec2 TexCoords;

uniform mat4 M = mat4(1.0f);
uniform mat4 V = mat4(1.0f);
uniform mat4 P = mat4(1.0f);

void main()
{
	gl_Position =  P * V * M * vec4( position, 1.0f );
	TexCoords = vec2( texCoords.x, 1.0f - texCoords.y );
}
