#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 color;
layout( location = 2 ) in vec2 texCoords;
layout( location = 3 ) in vec2 faceTexCoords;

out vec4 outColor;
out vec2 outTexCoords;
out vec2 outFaceTexCoords;

void main()
{
	gl_Position = vec4( position.x, position.y, position.z, 1.0 );
	outColor = vec4( color, 1.0f );
	outTexCoords = texCoords;
	outFaceTexCoords = faceTexCoords;
}
