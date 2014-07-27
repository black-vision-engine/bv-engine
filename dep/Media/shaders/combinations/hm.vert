#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

uniform mat4 MVP;

uniform float centerX;
uniform float centerY;

uniform float scaleX;
uniform float scaleY;

out vec2 uvCoord_hm;
out vec2 uvCoord_tx;

void main()
{
    gl_Position = MVP * vec4( vertexPosition, 1.0 );

    float u = centerX + 1.0 / scaleX * ( vertexTexCoord.x - 0.5 );
    float v = centerY + 1.0 / scaleY * ( vertexTexCoord.y - 0.5 );

    uvCoord_hm = vec2( u, v );
    uvCoord_tx = vertexTexCoord;
}
