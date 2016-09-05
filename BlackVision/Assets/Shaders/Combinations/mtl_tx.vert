#version 420

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;
uniform mat3 normalMatMV;

uniform mat4 txMat;

out vec3 position;	//vertex position in modelview space
out vec3 normal;	//vertex normal in modelview space
out vec2 uvCoord;

void main()
{
    gl_Position = MVP * vec4( vertexPosition, 1.0 );
	
	position = vec3( MV * vec4( vertexPosition, 1.0 ) );
	
	normal = normalMatMV * vertexNormal;
	
	uvCoord = ( txMat * vec4( vertexTexCoord, 0.0, 1.0 ) ).xy;
}
