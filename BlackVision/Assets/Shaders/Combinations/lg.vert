#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec2 vertexTexCoord;

uniform mat4 MVP;
uniform mat4 txMat;

out vec2 uvCoord;

void main()
{
    gl_Position =  MVP * vec4(vertexPosition, 1.0);
    uvCoord = ( txMat * vec4( vertexTexCoord, 0.0, 1.0 ) ).xy;
}
