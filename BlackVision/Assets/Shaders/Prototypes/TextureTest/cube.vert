#version 430 core

#define POSITION		0
#define TEXCOORD		2

layout( location = POSITION ) in vec3 Position;
layout( location = TEXCOORD ) in vec3 Texcoord;

out gl_PerVertex
{
	vec4 gl_Position;
};

out block
{
	vec3 Texcoord;
} Out;

layout( location = 0 ) uniform mat4 MVP;

void main()
{
	Out.Texcoord = Texcoord;
    gl_Position = MVP * vec4( Position, 1.0 );
}