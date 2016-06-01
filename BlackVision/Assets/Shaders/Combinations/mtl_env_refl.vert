#version 420

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform mat3 normalMatMV;
uniform mat3 normalMatM;

uniform mat4 reflMat;

layout( std140, binding = 1 ) uniform Camera
{
	uniform vec3 cameraPos;		// Camera in world space
};


out vec3 position;			//vertex position in modelview space
out vec3 normal;			//vertex normal in modelview space
out vec3 envReflection;		//reflection direction in world space
out vec2 uvCoordReflectivityMap;


vec3 ComputeEnvironmentReflectionVec();


void main()
{
    gl_Position = MVP * vec4( vertexPosition, 1.0 );
	
	position = vec3( MV * vec4( vertexPosition, 1.0 ) );
	
	uvCoordReflectivityMap = ( reflMat * vec4( vertexTexCoord, 0.0, 1.0 ) ).xy;
	normal = normalMatMV * vertexNormal;
	
	// Computing environmental reflection vector
	envReflection = ComputeEnvironmentReflectionVec();
}

vec3 ComputeEnvironmentReflectionVec()
{
	vec3 normalWorldSpace = normalMatM * vertexNormal;
	vec3 positionWorldSpace = vec3( M * vec4( vertexPosition, 1.0 ) );
	
	vec3 viewDirection = cameraPos - positionWorldSpace;
	return reflect( viewDirection, normalWorldSpace );
}
