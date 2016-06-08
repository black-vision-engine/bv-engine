#version 400

layout (location = 0) out vec4 FragColor;

in vec2			uvCoord;
in vec3 		envReflection;	//reflection direction in world space


uniform sampler2D Tex0;
uniform float alpha;


// *** ENVIRONMENTAL MAP ***
uniform sampler2D 	EnvMap0;
uniform float		reflectivity;

#define M_PI 3.1415926535897932384626433832795


vec3 computeEnvironment			( vec3 reflectionVec );



void main()
{
	vec3 envColor = computeEnvironment( envReflection );
	
	vec4 color = texture( Tex0, uvCoord );
	color = mix( color, vec4( envColor, 1.0 ), reflectivity );
	
	FragColor = color * alpha;
	//FragColor = col * col.a * alpha;  //This one uses premultiplied RGB values
}


vec3 computeEnvironment			( vec3 reflectionVec )
{
	reflectionVec = normalize( reflectionVec );
	
	vec2 uvCoord;
	uvCoord.x = atan( reflectionVec.z, reflectionVec.x ) / ( 2 * M_PI ) + 0.5;
	uvCoord.y = acos( reflectionVec.y ) / M_PI;
	
	vec3 texColor = texture( EnvMap0, uvCoord ).xyz;
	return texColor;
}