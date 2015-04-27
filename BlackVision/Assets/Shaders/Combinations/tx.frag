#version 450

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D Tex0;

uniform float alpha;

void main()
{
	vec4 col = texture( Tex0, uvCoord );
	FragColor = col * alpha; 
    int l = textureQueryLevels(Tex0);
    if( l == 11)
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    else
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    
//	FragColor = col; 
	//FragColor = col * col.a * alpha;  //This one uses premultiplied RGB values
}
