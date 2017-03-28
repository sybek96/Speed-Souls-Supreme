#version 400

uniform vec4 color;
uniform sampler2D texture;
uniform bool on;

void main( void ) 
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	if (on != true)
	{
		pixel = vec4(0.0, 0.0, 0.0, 0.0);
	}

	gl_FragColor = pixel * color;
	
}