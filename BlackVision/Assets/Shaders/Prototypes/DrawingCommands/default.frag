#version 400

in Fragment
{
	vec4 color;
} fragment;

layout ( location = 0 ) out vec4 color;

void main()
{		
	//color = fragment.color;
	color = vec4(0.45f,0.63f,0.72f,1);
}
