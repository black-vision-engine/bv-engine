#version 420

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec4 vertexTangent;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 V;
uniform mat3 normalMatMV;
uniform mat4 txMat;

uniform mat4 normalMapMat;

out vec3 position;	//vertex position in modelview space
out vec2 normUVCoord;
out vec2 uvCoord;
out mat3 TBN;    	//matrix transformation to tangent space


// *** ENVIRONMENTAL MAP ***
out mat3 tangentToWorldSpace;



void main()
{
    gl_Position = MVP * vec4( vertexPosition, 1.0 );
	
	position = vec3( MV * vec4( vertexPosition, 1.0 ) );
	
	vec3 N = normalize( normalMatMV * vertexNormal );
	vec3 T = normalize( normalMatMV * vec3( vertexTangent ) );
	vec3 B = normalize( cross( N, T ) ) * vertexTangent.w;
	
	TBN = mat3( T.x, B.x, N.x,
				T.y, B.y, N.y,
				T.z, B.z, N.z );
				
	// Compute world space to tangent space matrix and inverse it by transposing.
	tangentToWorldSpace = transpose( TBN * mat3( V ) );

	normUVCoord = ( normalMapMat * vec4( vertexTexCoord, 0.0, 1.0 ) ).xy;
	uvCoord = ( txMat * vec4( vertexTexCoord, 0.0, 1.0 ) ).xy;
}
