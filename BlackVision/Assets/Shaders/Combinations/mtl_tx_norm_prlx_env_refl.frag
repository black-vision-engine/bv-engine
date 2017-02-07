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

// *** LIGHTS ***

// MATERIAL
uniform vec4 		mtlDiffuse;
uniform vec4 		mtlAmbient;
uniform vec4 		mtlSpecular;
uniform vec4		mtlEmission;
uniform int 		mtlShininess;

uniform sampler2D 	Tex0;
uniform sampler2D 	NormMap0;
uniform sampler2D 	ParallaxMap0;

uniform float 		alpha;

uniform float 		heightScale;
uniform int 		minSamplesNum;
uniform int 		maxSamplesNum;

in vec3 			position;		//vertex position in modelview space
in vec2 			normUVCoord;
in mat3 			TBN;			//matrix transformation to tangent space
in mat4 			texMat;


vec3 computeDirectionalLight	( DirectionalLight light, vec3 viewDir, vec3 normal );
vec3 computePointLight			( PointLight light, vec3 viewDir, vec3 normal );
vec3 computeSpotLight			( SpotLight light, vec3 viewDir, vec3 normal );

// *** ENVIRONMENTAL MAP ***
in mat3 		tangentToWorldSpace;
in vec2			uvCoordReflectivityMap;

uniform sampler2D 	EnvMap0;
uniform sampler2D	ReflectivityMap0;
uniform float		reflectivity;

#define M_PI 3.1415926535897932384626433832795


vec3 computeEnvironment			( vec3 viewDir, vec3 normal );
float computeReflectivity		();

vec2 parallaxMapping			( vec3 viewDir );



void main()
{		
	vec3 viewDir = normalize( TBN * ( -position ) );
	
	vec2 texCoord = parallaxMapping( viewDir );
	if( texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0 )
        discard;
	
	vec3 norm = normalize( 2.0 * texture( NormMap0, texCoord ).rgb - 1.0 );
	norm.y = -norm.y; //flip y coord
	
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
	
	vec4 texColor = texture( Tex0, ( texMat * vec4( texCoord, 0.0, 1.0 ) ).xy );
	color = color * texColor.rgb * texColor.a;
	
	vec3 envColor = computeEnvironment( viewDir, norm );
	color = mix( color, envColor, computeReflectivity() );
	
	FragColor = vec4( color, texColor.a * alpha );
}

vec3 computeDirectionalLight	( DirectionalLight light, vec3 viewDir, vec3 norm )
{
	vec3 lightDir = normalize( TBN * ( -light.direction ) );
	
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
	vec3 lightDir = normalize( TBN * ( light.position - position ) );
	
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
	vec3 lightDir = normalize( TBN * ( light.position - position ) );
	
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

// parallax occlusion mapping
vec2 parallaxMapping			( vec3 viewDir )
{
    float currHeight = 0.0;
	vec2 currTex = normUVCoord;
	
    float samplesNum = mix( maxSamplesNum, minSamplesNum, abs( dot( vec3( 0, 0, 1 ), viewDir ) ) );
    float sampleHeight = 1.0 / samplesNum;
	
    vec2 stepHeight = ( heightScale * ( viewDir.xy / viewDir.z ) ) / samplesNum;
	
    float heightTex = texture( ParallaxMap0, currTex ).r;
      
    while( currHeight < heightTex )
    {
        currTex -= stepHeight;
        heightTex = texture( ParallaxMap0, currTex ).r;  
        currHeight += sampleHeight;  
    }
    
    vec2 prevTex = currTex + stepHeight;

    float nextHeight  = heightTex - currHeight;
    float prevHeight = texture( ParallaxMap0, prevTex ).r - currHeight + sampleHeight;
 
    float weight = nextHeight / ( nextHeight - prevHeight );
	
    return ( prevTex * weight + currTex * ( 1.0 - weight ) );
}


// viewDir and normal should be already normalized.
vec3 computeEnvironment			( vec3 viewDir, vec3 normal )
{
	vec3 reflectionTangentSpace = reflect( viewDir, normal );
	vec3 reflectionVec = tangentToWorldSpace * reflectionTangentSpace;
	
	vec2 uvCoord;
	uvCoord.x = atan( reflectionVec.z, reflectionVec.x ) / ( 2 * M_PI ) + 0.5;
	uvCoord.y = acos( reflectionVec.y ) / M_PI;
	
	vec3 texColor = texture( EnvMap0, uvCoord ).xyz;
	
	return texColor;
}

float computeReflectivity		()
{
	return reflectivity * texture( ReflectivityMap0, uvCoordReflectivityMap ).x;
}