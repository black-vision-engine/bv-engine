#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

uniform mat4 MVP;

out vec2 texCord;

void main()
{
    gl_Position = MVP * vec4(vertexPosition, 1.0);
	texCord = vertexTexCoord;
}
