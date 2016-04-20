#version 430

layout (location = 0) in vec3 vertexPosition;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4( vertexPosition, 1.0 );
	gl_Position.z = 0.0;
}
