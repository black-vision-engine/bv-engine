#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 instance_color;

out Fragment
{
	vec4 color;
} fragment;

void main()
{
    gl_Position =  vec4(position, 1.0);
	fragment.color = instance_color;
}
