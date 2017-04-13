#version 420

layout (location = 0) out vec4 FragColor;

// *** LIGHTS *** (Assets/Shaders/UniformBlockDefs/lights)

#define MAX_LIGTHS 8

struct DirectionalLight
{
	vec3 	color;
    vec3 	direction;
};

struct PointLight
{
	vec3 	color;
	vec3	position;
	vec3 	attenuation;
};

struct SpotLight
{
	vec3 	color;
	vec3 	direction;
	vec3 	position;
	vec3 	attenuation;
	
	float 	cutOff;
	int 	exponent;
};


layout ( std140, binding = 0 ) uniform Lights 
{
	uniform	DirectionalLight[ MAX_LIGTHS ] 	directionalLight;
	uniform int 							directionalLightNum;
	uniform PointLight[ MAX_LIGTHS ] 		pointLight;
	uniform int 							pointLightNum;	
	uniform SpotLight[ MAX_LIGTHS ] 		spotLight;
	uniform int 							spotLightNum;
};

layout( std140, binding = 1 ) uniform Camera
{
	uniform vec3 cameraPos;
};

// *** LIGHTS ***

// MATERIAL
uniform vec4 		mtlDiffuse;
uniform vec4 		mtlAmbient;
uniform vec4 		mtlSpecular;
uniform vec4 		mtlEmission;
uniform int 		mtlShininess;

// uniform float 		alpha;


in vec3 		position;		//vertex position in modelview space
in vec3 		normal;			//vertex normal in modelview space
in vec3 		envReflection;	//reflection direction in world space
in vec2			uvCoordReflectivityMap;

// *** ENVIRONMENTAL MAP ***
uniform sampler2D 	EnvMap0;
uniform sampler2D	ReflectivityMap0;
uniform float		reflectivity;
uniform mat4 		envMat;	
uniform int			envMixMode;


#define M_PI 3.1415926535897932384626433832795


vec3 computeEnvironment			( vec3 reflectionVec );
vec3 mixWithEnvironment			( vec3 color, vec3 envColor, float reflectivityFactor );
float computeReflectivity		();

vec3 computeDirectionalLight	( DirectionalLight light, vec3 viewDir, vec3 normal );
vec3 computePointLight			( PointLight light, vec3 viewDir, vec3 normal );
vec3 computeSpotLight			( SpotLight light, vec3 viewDir, vec3 normal );


void main()
{		
	vec3 viewDir = normalize( -position );
	vec3 norm = normalize( normal );
	
	vec3 color = vec3( 0, 0, 0 );
	
	for( int i = 0; i < directionalLightNum; ++i )
	{
		color += computeDirectionalLight( directionalLight[ i ], viewDir, norm );
	}

	for( int i = 0; i < pointLightNum; ++i )
	{
		color += computePointLight( pointLight[ i ], viewDir, norm );
	}
	
	for( int i = 0; i < spotLightNum; ++i )
	{
		color += computeSpotLight( spotLight[ i ], viewDir, norm );
	}
	
	vec3 emission = mtlEmission.rgb * mtlEmission.a;
	color += emission;
	
	vec3 envColor = computeEnvironment( envReflection );
	color = mixWithEnvironment( color, envColor, computeReflectivity() );
	
	FragColor = vec4( color, 1.0 );
}

vec3 computeDirectionalLight	( DirectionalLight light, vec3 viewDir, vec3 norm )
{
	vec3 lightDir = normalize( -light.direction );
	
	float diffuseCoeff = max( dot( norm, lightDir ), 0.0 );
	float specularCoeff = 0.0;
	if( diffuseCoeff > 0.0 )
	{
		vec3 halfDir = normalize( viewDir + lightDir );
		specularCoeff = pow( max( dot( norm, halfDir ), 0.0 ), mtlShininess );
	}
	
	vec3 diffuse = diffuseCoeff * mtlDiffuse.rgb * light.color * mtlDiffuse.a;
	vec3 specular = specularCoeff * mtlSpecular.rgb * light.color * mtlSpecular.a;
	vec3 ambient = mtlAmbient.rgb * light.color * mtlAmbient.a;
	
	return ambient + diffuse + specular;	
}

vec3 computePointLight			( PointLight light, vec3 viewDir, vec3 norm )
{
	vec3 lightDir = normalize( light.position - position );
	
	float diffuseCoeff = max( dot( norm, lightDir ), 0.0 );
	float specularCoeff = 0.0;
	if( diffuseCoeff > 0.0 )
	{
		vec3 halfDir = normalize( viewDir + lightDir );
		specularCoeff = pow( max( dot( norm, halfDir ), 0.0 ), mtlShininess );
	}
	
	float distance = length( light.position - position );
	float attenuation = 1.0 / ( light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * distance * distance );
	
	vec3 diffuse = diffuseCoeff * mtlDiffuse.rgb * light.color * mtlDiffuse.a;
	vec3 specular = specularCoeff * mtlSpecular.rgb * light.color * mtlSpecular.a;
	vec3 ambient = mtlAmbient.rgb * light.color * mtlAmbient.a;
	
	return attenuation * ( ambient + diffuse + specular );	
}

vec3 computeSpotLight			( SpotLight light, vec3 viewDir, vec3 norm )
{
	vec3 lightDir = normalize( light.position - position );
	
	float diffuseCoeff = max( dot( norm, lightDir ), 0.0 );
	float specularCoeff = 0.0;
	if( diffuseCoeff > 0.0 )
	{
		vec3 halfDir = normalize( viewDir + lightDir );
		specularCoeff = pow( max( dot( norm, halfDir ), 0.0 ), mtlShininess );
	}
	
	float distance = length( light.position - position );
	float attenuation = 1.0 / ( light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * distance * distance );
	
	vec3 diffuse = diffuseCoeff * mtlDiffuse.rgb * light.color * mtlDiffuse.a;
	vec3 specular = specularCoeff * mtlSpecular.rgb * light.color * mtlSpecular.a;
	vec3 ambient = mtlAmbient.rgb * light.color * mtlAmbient.a;
	
	float dl = max( 0.0, dot( -lightDir, light.direction ) );
	
	attenuation *= step( light.cutOff, dl ) * pow( dl, light.exponent );
	
	return attenuation * ( ambient + diffuse + specular );
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

float computeReflectivity		()
{
	return reflectivity * texture( ReflectivityMap0, uvCoordReflectivityMap ).x;
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