#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 normalMat;

out vec3 position;	//vertex position in modelview space
out vec3 normal;	//vertex normal in modelview space


void main()
{
    gl_Position = MVP * vec4( vertexPosition, 1.0 );
	
	position = vec3( MV * vec4( vertexPosition, 1.0 ) );
	
	normal = normalize( normalMat * vertexNormal );	
}
