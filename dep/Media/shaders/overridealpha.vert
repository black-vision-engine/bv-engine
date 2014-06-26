#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;

out vec2 uvCoord;

void main()
{
    gl_Position = MVP * vec4( vertexPosition, 1.0 );
    uvCoord = vertexTexCoord;
}
