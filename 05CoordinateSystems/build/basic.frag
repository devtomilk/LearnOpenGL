#version 330 core

out vec4 color;

in vec2 TexCoords;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main()
{
	color = mix( texture(myTexture1, TexCoords), texture(myTexture2, TexCoords), 0.2f );
}
