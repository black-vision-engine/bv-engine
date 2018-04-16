#version 420

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform float tone;

layout (binding = 0) uniform sampler2D Texture;


float Luminance( in vec4 color )
{
    return (color.r + color.g + color.b ) / 3.0;
}

vec4 Sepia( in vec4 color )
{
    return vec4(
          clamp(color.r * 0.393 + color.g * 0.769 + color.b * 0.189, 0.0, 1.0)
        , clamp(color.r * 0.349 + color.g * 0.686 + color.b * 0.168, 0.0, 1.0)
        , clamp(color.r * 0.272 + color.g * 0.534 + color.b * 0.131, 0.0, 1.0)
        , color.a
    );
}

void main (void){
	float factor = tone/100;
	vec4 col = texture2D( Texture, uvCoord);
	FragColor = mix(col, Sepia(col), clamp(factor,0.0,1.0) );
}
