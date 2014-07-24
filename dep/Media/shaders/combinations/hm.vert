#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;

uniform float offsetX;
uniform float scaleX;

out vec2 uvCoord;

void main()
{
    gl_Position = MVP * vec4( vertexPosition, 1.0 );
    //uvCoord = vec2( vertexTexCoord.x * 0.1, vertexTexCoord.y );
    float u = vertexTexCoord.x / scaleX + offsetX;
    float v = vertexTexCoord.y;

    uvCoord = vec2( u, v );
}
