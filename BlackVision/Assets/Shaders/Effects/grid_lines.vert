#version 430

layout (location = 0) in vec3 vertexPosition;

out vec3 VPosition;

void main()
{
    gl_Position = vec4( vertexPosition, 1.0 );
}
