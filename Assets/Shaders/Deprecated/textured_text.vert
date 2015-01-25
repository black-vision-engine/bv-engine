#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexAtlasCoord;
layout (location = 2) in vec2 vertexTex0Coord;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;

out vec2 VAtlasTexCord;
out vec2 VTex0Cord;

void main()
{
	vec4 dummy = MV * P * vec4(vertexPosition, 1.0);
    gl_Position = MVP * vec4(vertexPosition, 1.0) + dummy - MV * P * vec4(vertexPosition, 1.0);
    VAtlasTexCord = vertexAtlasCoord;
	VTex0Cord = vertexTex0Coord;
}
