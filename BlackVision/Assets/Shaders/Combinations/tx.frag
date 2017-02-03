#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D Tex0;

uniform float alpha;


void main()
{
	vec4 col = texture( Tex0, uvCoord );
	//FragColor = col * alpha;
	FragColor = col * col.a * alpha;  //This one uses premultiplied RGB values
}
