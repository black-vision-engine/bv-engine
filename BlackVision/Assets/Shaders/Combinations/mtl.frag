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
uniform vec4 	mtlDiffuse;
uniform vec4 	mtlAmbient;
uniform vec4 	mtlSpecular;
uniform vec4 	mtlEmission;
uniform int 	mtlShininess;


in vec3 		position;		//vertex position in modelview space
in vec3 		normal;			//vertex normal in modelview space


vec3 computeDirectionalLight	( DirectionalLight light, vec3 viewDir );
vec3 computePointLight			( PointLight light, vec3 viewDir );
vec3 computeSpotLight			( SpotLight light, vec3 viewDir );


void main()
{		
	vec3 viewDir = normalize( -position );
	
	vec3 color = vec3( 0, 0, 0 );
	
	for( int i = 0; i < directionalLightNum; ++i )
	{
		color += computeDirectionalLight( directionalLight[ i ], viewDir );
	}

	for( int i = 0; i < pointLightNum; ++i )
	{
		color += computePointLight( pointLight[ i ], viewDir );
	}
	
	for( int i = 0; i < spotLightNum; ++i )
	{
		color += computeSpotLight( spotLight[ i ], viewDir );
	}
	
	vec3 emission = mtlEmission.rgb * mtlEmission.a;
	
	FragColor = vec4( emission + color, 1.0 );
}

vec3 computeDirectionalLight	( DirectionalLight light, vec3 viewDir )
{
	vec3 lightDir = normalize( -light.direction );
	
	float diffuseCoeff = max( dot( normal, lightDir ), 0.0 );
	float specularCoeff = 0.0;
	if( diffuseCoeff > 0.0 )
	{
		vec3 halfDir = normalize( viewDir + lightDir );
		specularCoeff = pow( max( dot( normal, halfDir ), 0.0 ), mtlShininess );
	}
	
	vec3 diffuse = diffuseCoeff * mtlDiffuse.rgb * light.color * mtlDiffuse.a;
	vec3 specular = specularCoeff * mtlSpecular.rgb * light.color * mtlSpecular.a;
	vec3 ambient = mtlAmbient.rgb * light.color * mtlAmbient.a;
	
	return ambient + diffuse + specular;	
}

vec3 computePointLight			( PointLight light, vec3 viewDir )
{
	vec3 lightDir = normalize( light.position - position );
	
	float diffuseCoeff = max( dot( normal, lightDir ), 0.0 );
	float specularCoeff = 0.0;
	if( diffuseCoeff > 0.0 )
	{
		vec3 halfDir = normalize( viewDir + lightDir );
		specularCoeff = pow( max( dot( normal, halfDir ), 0.0 ), mtlShininess );
	}
	
	float distance = length( light.position - position );
	float attenuation = 1.0 / ( light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * distance * distance );
	
	vec3 diffuse = diffuseCoeff * mtlDiffuse.rgb * light.color * mtlDiffuse.a;
	vec3 specular = specularCoeff * mtlSpecular.rgb * light.color * mtlSpecular.a;
	vec3 ambient = mtlAmbient.rgb * light.color * mtlAmbient.a;
	
	return attenuation * ( ambient + diffuse + specular );	
}

vec3 computeSpotLight			( SpotLight light, vec3 viewDir )
{
	vec3 lightDir = normalize( light.position - position );
	
	float diffuseCoeff = max( dot( normal, lightDir ), 0.0 );
	float specularCoeff = 0.0;
	if( diffuseCoeff > 0.0 )
	{
		vec3 halfDir = normalize( viewDir + lightDir );
		specularCoeff = pow( max( dot( normal, halfDir ), 0.0 ), mtlShininess );
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
