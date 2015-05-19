#version 400

in Fragment
{
	vec4 color;
} fragment;

layout ( location = 0 ) out vec4 color;

void main()
{		
	color = fragment.color;	
}
