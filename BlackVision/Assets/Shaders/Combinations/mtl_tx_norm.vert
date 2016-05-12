#version 420

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec4 vertexTangent;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 normalMat;

uniform mat4 txMat;
uniform mat4 normalMapMat;

out vec3 position;	//vertex position in modelview space
out vec2 uvCoord;
out vec2 normUVCoord;
out mat3 TBN;    	//matrix transformation to tangent space


void main()
{
    gl_Position = MVP * vec4( vertexPosition, 1.0 );
	
	position = vec3( MV * vec4( vertexPosition, 1.0 ) );
	
	vec3 N = normalize( normalMat * vertexNormal );
	vec3 T = normalize( normalMat * vec3( vertexTangent ) );
	vec3 B = normalize( cross( N, T ) ) * vertexTangent.w;
	
	TBN = mat3( T.x, B.x, N.x,
				T.y, B.y, N.y,
				T.z, B.z, N.z );
	
	uvCoord = ( txMat * vec4( vertexTexCoord, 0.0, 1.0 ) ).xy;
	normUVCoord = ( normalMapMat * vec4( vertexTexCoord, 0.0, 1.0 ) ).xy;
}
