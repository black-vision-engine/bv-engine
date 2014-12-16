#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D Tex0;

uniform float alpha;

void main()
{
	float col = texture( Tex0, uvCoord );
	FragColor = vec4( 1.0, 1.0, 1.0, 1.0 ) * col * alpha; 
	//FragColor = col * col.a * alpha;  //This one uses premultiplied RGB values
}
