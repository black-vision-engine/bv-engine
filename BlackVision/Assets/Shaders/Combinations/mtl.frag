#version 400

layout (location = 0) out vec4 FragColor;

in vec3 position;
in vec3 normal;
in vec2 uvCoord;

uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec4 specular;
uniform vec4 emission;
uniform float shininess;

struct DirectionalLight
{
	vec3 ambient;
	vec3 color;
    vec3 direction;
};

struct PointLight
{
	vec3 ambient;
	vec3 color;
	float attenuation;
	vec3 position;
};

struct SpotLight
{
	vec3 ambient;
	vec3 color;
	vec3 position;
    vec3 direction;	
	float attenuation;
	float cutOff;
	float outerCutOff;
};

vec3 computeDirectionalLight( DirectionalLight light )
{
	vec3 viewDir = normalize( -position );
	vec3 lightDir = normalize( -light.direction );
	
	float diffuseCoeff = max( dot( normal, lightDir ), 0.0 );
	float specularCoeff = 0.0f;
	if( diffuseCoeff > 0.0 )
	{
		vec3 halfDir = normalize( viewDir + lightDir );
		specularCoeff = pow( max( dot( normal, halfDir ), 0.0 ), shininess );
	}
	
	vec3 diffuse_ = diffuseCoeff * diffuse.rgb * light.color * diffuse.a;
	vec3 specular_ = specularCoeff * specular.rgb * light.color * specular.a;
	vec3 ambient_ = ambient.rgb * ambient.a * light.ambient;
	
	return ambient_ + diffuse_ + specular_;	
}

vec3 computePointLight( PointLight light )
{
	vec3 viewDir = normalize( -position );
	vec3 lightDir = normalize( light.position - position );
	
	float diffuseCoeff = max( dot( normal, lightDir ), 0.0 );
	float specularCoeff = 0.0f;
	if( diffuseCoeff > 0.0 )
	{
		vec3 halfDir = normalize( viewDir + lightDir );
		specularCoeff = pow( max( dot( normal, halfDir ), 0.0 ), shininess );
	}
	
	float distance = length( light.position - position );
	float attenuation = 1.0 / ( 1.0 + light.attenuation * pow( distance, 2 ) ); 
	
	vec3 diffuse_ = diffuseCoeff * diffuse.rgb * light.color * diffuse.a;
	vec3 specular_ = specularCoeff * specular.rgb * light.color * specular.a;
	vec3 ambient_ = ambient.rgb * ambient.a * light.ambient;
	
	return attenuation * ( ambient_ + diffuse_ + specular_ );	
}

vec3 computeSpotLight( SpotLight light )
{
	vec3 viewDir = normalize( -position );
	vec3 lightDir = normalize( light.position - position );
	
	float diffuseCoeff = max( dot( normal, lightDir ), 0.0 );
	float specularCoeff = 0.0f;
	if( diffuseCoeff > 0.0 )
	{
		vec3 halfDir = normalize( viewDir + lightDir );
		specularCoeff = pow( max( dot( normal, halfDir ), 0.0 ), shininess );
	}
	
	float distance = length( light.position - position );
	float attenuation = 1.0 / ( 1.0 + light.attenuation * pow( distance, 2 ) ); 
		
	float theta = dot( lightDir, normalize( -light.direction ) );
	float eps = light.cutOff - light.outerCutOff;
	float intensity = clamp( ( theta - light.outerCutOff ) / eps, 0.0, 1.0 );
	
	vec3 diffuse_ = diffuseCoeff * diffuse.rgb * light.color * diffuse.a;
	vec3 specular_ = specularCoeff * specular.rgb * light.color * specular.a;
	vec3 ambient_ = ambient.rgb * ambient.a * light.ambient;
	
	return attenuation * intensity * ( ambient_ + diffuse_ + specular_ );
}

void main()
{		
//	DirectionalLight light;
//	PointLight light;
	SpotLight light;

	light.color = vec3( 1, 1, 1 );
	light.ambient = vec3( 0.5, 0.5, 0.5 );
	
	light.direction = vec3( -1, 0, 0 );
	
	light.position = vec3( 5, 0, -2.5 );	
	light.attenuation = 0.02;
	
	light.cutOff = cos( 12.5 * 3.14 / 180.0 );
	light.outerCutOff = cos( 15.0 * 3.14 / 180.0 );
	
//	vec3 lightColor = computeDirectionalLight( light );
//	vec3 lightColor = computePointLight( light );
	vec3 lightColor = computeSpotLight( light );
	
	vec3 emission_ = emission.rgb * emission.a;
	
	FragColor = vec4( emission_ + lightColor, 1.0 );
}
