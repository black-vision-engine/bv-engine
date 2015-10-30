#version 430

layout (location = 0) in vec3 vertexPosition;

uniform mat4 MVP;
uniform mat4 MV;

out vec3 VPosition;

void main()
{
    VPosition = vec3( MV * vec4( VertexPosition, 1.0 ) );

    gl_Position = MVP * vec4( vertexPosition, 1.0 );
}
