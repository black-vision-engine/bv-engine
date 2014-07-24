#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

uniform mat4 MVP;

uniform float offsetX;
uniform float offsetY;
uniform float windowWidth;
uniform float windowHeight;

out vec2 uvCoord_hm;
out vec2 uvCoord_tx;

void main()
{
    gl_Position = MVP * vec4( vertexPosition, 1.0 );

    float u = vertexTexCoord.x * windowWidth + offsetX;
    float v = vertexTexCoord.y * windowHeight + offsetY;

    uvCoord_hm = vec2( u, v );
    uvCoord_tx = vec2( u, v );
}
