#version 430

layout (location = 0) out vec4 FragColor;

in                  vec3 GPosition;
noperspective in    vec3 GEdgeDistance;

void main()
{
    // Some default values
    // TODO: move them to uniform variables
    float lineWidth = 2.5f;
    vec4 lineColor = vec4( 0.0, 0.0, 0.0, 1.0 ); // black line
    vec4 mainColor = vec4( 1.0, 1.0, 1.0, 1.0 ); // white background

    // Find the smallest distance
    float d = min( min( GEdgeDistance.x, GEdgeDistance.y ), GEdgeDistance.z );

    // Determine the mix factor with the line color
    float mixVal = smoothstep( lineWidth - 1, lineWidth + 1, d );

    FragColor = mix( lineColor, mainColor, mixVal );
}
