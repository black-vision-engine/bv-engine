#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

uniform mat4 txMat;

out vec2 uvCoord;

void main()
{
    gl_Position = vec4( vertexPosition, 1.0 );
    uvCoord = vertexTexCoord;
}