#version 330 core

out vec4 color;

in vec3 out_position;

void main()
{
	//color = vec4( 1.0f, 0.5f, 0.2f, 1.0f );
	color = vec4( out_position.x + 0.5, 
			      out_position.y + 0.5,
				  out_position.z + 0.5,
				  1.0f );
}
