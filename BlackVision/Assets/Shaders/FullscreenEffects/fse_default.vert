#version 400

in int gl_VertexID;

const vec4 quadVerticies [ 4 ] = vec4[]( vec4( -1.0, -1.0, 0.0, 1.0 ), vec4( 1.0, -1.0, 0.0, 1.0 ), vec4( -1.0, 1.0, 0.0, 1.0 ), vec4( 1.0, 1.0, 0.0, 1.0 ) );
const vec2 usCoords [ 4 ] = vec2[]( vec2( 0.0, 0.0 ), vec2( 1.0, 0.0 ), vec2(  0.0, 1.0 ), vec2( 1.0, 1.0 ) );

uniform vec4 quadTransform = vec4( 1.0, 1.0, 0.0, 0.0 );
uniform vec4 uvTransform = vec4( 1.0, 1.0, 0.0, 0.0 );

out vec2 uvCoord;

void main()
{
	vec2 uv = vec2( usCoords[ gl_VertexID ].x * uvTransform.x, usCoords[ gl_VertexID ].y * uvTransform.y );
	uv = vec2( uv.x + uvTransform.z, uv.y + uvTransform.w );
	
	uvCoord = uv;
	
	vec4 pos = vec4( ( quadVerticies[ gl_VertexID ].x + 1.f ) * quadTransform.x, ( quadVerticies[ gl_VertexID ].y + 1.f ) * quadTransform.y, quadVerticies[ gl_VertexID ].z, quadVerticies[ gl_VertexID ].w );
	pos = vec4( pos.x - 1.f, pos.y - 1.f, pos.z, pos.w );
	pos = vec4( pos.x + quadTransform.z, pos.y + quadTransform.w, pos.z, pos.w );
	
    gl_Position =  pos;
}
