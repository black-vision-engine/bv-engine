#version 420


struct DirectionalLight
{
	vec3 	color;
    vec3 	direction;
};

struct PointLight
{
	vec3 	color;
	vec3	position;
	
	float 	attConstant;
	float 	attLinear;
	float 	attQuadratic;
};

struct SpotLight
{
	vec3 	color;
	vec3 	direction;
	vec3 	position;
	
	float 	attConstant;
	float 	attLinear;
	float 	attQuadratic;
	
	float 	cutOff;
	float 	outerCutOff;
};


#define MAX_LIGTHS 8

layout (location = 0) out vec4 FragColor;

// LIGHTS
layout ( std140, binding = 0 ) uniform Lights 
{
	uniform	DirectionalLight[ MAX_LIGTHS ] 	directionalLight;
	uniform int 							directionalLightNum;
	uniform PointLight[ MAX_LIGTHS ] 		pointLight;
	uniform int 							pointLightNum;	
	uniform SpotLight[ MAX_LIGTHS ] 		spotLight;
	uniform int 							spotLightNum;
};

// MATERIAL
uniform vec4 	mtlDiffuse;
uniform vec4 	mtlAmbient;
uniform vec4 	mtlSpecular;
uniform vec4 	mtlEmission;
uniform float 	mtlShininess;


in vec3 		position;		//vertex position in modelview space
in vec3 		normal;			//vertex normal in modelview space
in vec2 		uvCoord;

uniform sampler2D Tex0;

uniform float alpha;

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
	vec4 texColor = texture( Tex0, uvCoord );
	
	FragColor = vec4( ( emission + color ) * texColor.rgb, texColor.a * alpha );
	
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
	float attenuation = 1.0 / ( light.attConstant + light.attLinear * distance + light.attQuadratic * pow( distance, 2 ) );
	
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
	float attenuation = 1.0 / ( light.attConstant + light.attLinear * distance + light.attQuadratic * pow( distance, 2 ) );
		
	float theta = dot( normalize( -light.direction ), lightDir );
	float eps = light.cutOff - light.outerCutOff;
	float intensity = clamp( ( theta - light.outerCutOff ) / eps, 0.0, 1.0 );
	
	vec3 diffuse = diffuseCoeff * mtlDiffuse.rgb * light.color * mtlDiffuse.a;
	vec3 specular = specularCoeff * mtlSpecular.rgb * light.color * mtlSpecular.a;
	vec3 ambient = mtlAmbient.rgb * light.color * mtlAmbient.a;
	
	return attenuation * intensity * ( ambient + diffuse + specular );
}
