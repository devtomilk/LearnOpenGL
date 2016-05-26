#version 330 core

out vec4 color;

in vec4 outColor;
in vec2 outTexCoords;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	//color = vec4( 1.0f, 0.5f, 0.2f, 1.0f );
	//color = texture( ourTexture, outTexCoords ) * outColor;
	color = mix(texture(ourTexture1, outTexCoords),
				texture(ourTexture2, vec2(outTexCoords.x,
										  -outTexCoords.y)),
				0.2);
}
