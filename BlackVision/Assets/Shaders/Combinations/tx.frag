#version 400

layout (location = 0) out vec4 FragColor;

in vec3 position;
in vec3 normal;
in vec2 uvCoord;

uniform sampler2D Tex0;

uniform float alpha;

vec3 lightPosition = vec3(5,5,0);
vec3 lightColor = vec3(1,1,1);

void main()
{
	vec4 col = texture( Tex0, uvCoord );
	
	vec3 surfaceToLight = lightPosition - position;
	float brightness = dot( normal, surfaceToLight ) / ( length( surfaceToLight ) * length( normal ) );
    brightness = clamp( brightness, 0, 1 );
	
	FragColor = col * alpha;
	FragColor.rgb *= brightness * lightColor;
	
	//FragColor = col * col.a * alpha;  //This one uses premultiplied RGB values
}
