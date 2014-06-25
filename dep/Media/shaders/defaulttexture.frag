#version 400

layout (location = 0) out vec4 FragColor;

//layout (location = 0) out vec3 FragColor;
//layout (location = 1) out float AlphaColor;
//layout (location = 2) out float AlphaColorRed;


in vec2 uvCoord;

uniform sampler2D Tex0;
uniform float alpha;
uniform float overrideAlpha;

void main()
{
	vec4 col = texture( Tex0, uvCoord );
	FragColor = col.rgba * alpha * overrideAlpha;
	
    //FragColor = col.rgb * alpha * overrideAlpha;
	//AlphaColor = col.a * alpha * overrideAlpha;
	//AlphaColorRed = col.b;
}
