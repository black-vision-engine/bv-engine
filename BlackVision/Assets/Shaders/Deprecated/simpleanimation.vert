#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;

out vec2 VTexCord;

void main()
{
	vec4 dummy = MV * P * vec4(vertexPosition, 1.0);
    gl_Position = MVP * vec4(vertexPosition, 1.0) + dummy - MV * P * vec4(vertexPosition, 1.0);
    VTexCord = vertexTexCoord;
}
