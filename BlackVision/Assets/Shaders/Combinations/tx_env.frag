#version 420

layout (location = 0) out vec4 FragColor;

in vec2			uvCoord;
in vec3 		envReflection;	//reflection direction in world space


uniform sampler2D Tex0;
uniform float alpha;


// *** ENVIRONMENTAL MAP ***
uniform sampler2D 	EnvMap0;
uniform float		reflectivity;
uniform mat4 		envMat;	
uniform int			envMixMode;

vec3 computeEnvironment			( vec3 reflectionVec );
vec3 mixWithEnvironment			( vec3 color, vec3 envColor, float reflectivityFactor );


#define M_PI 3.1415926535897932384626433832795


void main()
{
	vec3 envColor = computeEnvironment( envReflection );
	
	vec4 color = texture( Tex0, uvCoord );
	color.xyz = mixWithEnvironment( color.xyz, envColor, reflectivity );
	
	FragColor = color * alpha;
	//FragColor = col * col.a * alpha;  //This one uses premultiplied RGB values
}



vec3 computeEnvironment			( vec3 reflectionVec )
{
	reflectionVec = normalize( reflectionVec );
	
	vec2 uvCoord;
	uvCoord.x = atan( reflectionVec.z, reflectionVec.x ) / ( 2 * M_PI ) + 0.5;
	uvCoord.y = acos( reflectionVec.y ) / M_PI;
	
	uvCoord = ( envMat * vec4( uvCoord.x, uvCoord.y, 0.0, 1.0 ) ).xy;
	
	vec3 texColor = texture( EnvMap0, uvCoord ).xyz;
	return texColor;
}


vec3 mixWithEnvironment			( vec3 color, vec3 envColor, float reflectivityFactor )
{
#define ENV_BLEND 0
#define ENV_DECAL 1
#define ENV_MODULATE 2
#define ENV_ADD 3
#define ENV_AVERAGE 4
#define ENV_ADD_SIGNED 5

	if( envMixMode == ENV_BLEND )
	{
		return mix( color, envColor, reflectivityFactor );
	}
	else if( envMixMode == ENV_DECAL )
	{
		return envColor * reflectivityFactor;
	}
	else if( envMixMode == ENV_MODULATE )
	{
		return color * envColor;
	}
	else if( envMixMode == ENV_ADD )
	{
		return color + envColor * reflectivityFactor;
	}
	else if( envMixMode == ENV_AVERAGE )
	{
		return ( color + envColor ) / 2.0f;
	}
	else if( envMixMode == ENV_ADD_SIGNED )
	{
		return color + envColor * reflectivityFactor - vec3( 0.5f, 0.5f, 0.5f );
	}
	else 
	{
		return color;
	}

#undef ENV_BLEND
#undef ENV_DECAL
#undef ENV_MODULATE
#undef ENV_ADD
#undef ENV_AVERAGE
#undef ENV_ADD_SIGNED
}


