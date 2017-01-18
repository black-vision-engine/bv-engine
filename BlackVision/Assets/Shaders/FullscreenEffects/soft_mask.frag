#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       Tex0;

uniform float           blend;
uniform vec2            position;
uniform vec2            scale;
uniform int             invert;
uniform int             alphaOnly;
uniform int             maskAspect;
uniform int             softMask;
uniform int             onlyMask = 0;
uniform int             onlyObject = 0;

uniform mat4            maskTx;


void main()
{   
    FragColor = texture( Tex0, uvCoord ) * alpha;

}
