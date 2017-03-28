#version 400

uniform vec4 color;
uniform sampler2D texture;
uniform float timeDur;
uniform float timeElapsed;

void main( void ) 
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	pixel.a = pixel.a * (timeElapsed / timeDur);
	
	gl_FragColor = pixel * color;
	
}