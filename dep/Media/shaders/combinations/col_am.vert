#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexAlphaTexCoord;

uniform mat4 MVP;
uniform mat4 txAlphaMat;

out vec2 uvAlphaCoord;

void main()
{
    gl_Position =  MVP * vec4(vertexPosition, 1.0);
    uvAlphaCoord = ( txAlphaMat * vec4( vertexAlphaTexCoord, 0.0, 1.0 ) ).xy;
}
