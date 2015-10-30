#version 430

layout ( triangles ) in;
layout ( triangle_strip, max_vertices = 3 ) out;

out					vec3 GPosition;
noperspective out	vec3 GEdgeDistance;

in vec3 VPosition[];

uniform mat4 ViewportMatrix;

vec3 viewport_transform( int idx )
{
	return vec3( ViewportMatrix * ( gl_in[ idx ].gl_Position / gl_in[ idx ].gl_Position.w ) );
}

vec3 find_altitudes( vec3 p0, vec3 p1, vec3 p2 )
{
	float a = length( p1 - p2 );
	float b = length( p2 - p0 );
	float c = length( p1 - p0 );

	float alpha = acos( ( b * b + c * c - a * a ) / ( 2.0 * b * c ) );
	float beta  = acos( ( a * a + c * c - b * b ) / ( 2.0 * a * c ) );

	float ha = abs( c * sin( beta ) );
	float hb = abs( c * sin( alpha ) );
	float hc = abs( b * sin( alpha ) );

	return vec3( ha, hb, hc );
}

void process_next_vertex( vec3 edgeDistances, int idx )
{
	GEdgeDistance = edgeDistances[ idx ];
	GPosition = VPosition[ idx ];
	gl_Position = gl_in[ idx ].gl_Position;

	EmitVertex();
}

void main()
{
	//Transform each vertex into viewport space
	vec3 p0 = viewport_transform( 0 );
	vec3 p1 = viewport_transform( 1 );
	vec3 p2 = viewport_transform( 2 );

	//Find the altitudes (ha, hb and hc)
	vec3 alts = find_altitudes( p0, p1, p2 );

	// Send triangle along with edge distances
	process_next_vertex( alts, 0 );
	process_next_vertex( alts, 1 );
	process_next_vertex( alts, 2 );

	EndPrimitive();
}
