#version 330 core

layout( location = 0 ) in vec3 position;

uniform float horizontalTranslation;

void main()
{
	gl_Position = vec4( position.x + horizontalTranslation, -position.y, position.z, 1.0 );
}
