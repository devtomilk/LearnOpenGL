#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 color;
layout( location = 2 ) in vec2 texCoords;

out vec4 outColor;
out vec2 outTexCoords;

uniform mat4 M = mat4(1.0);
uniform mat4 V = mat4(1.0);
uniform mat4 P = mat4(1.0);

void main()
{
	gl_Position = P * V * M * vec4( position.x, position.y, position.z, 1.0 );
	outColor = vec4( color, 1.0f );
	outTexCoords = texCoords;
}
