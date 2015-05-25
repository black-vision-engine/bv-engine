#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 instance_color;
layout (location = 2) in vec4 instance_position;

out Fragment
{
	vec4 color;
} fragment;

void main()
{
	vec3 offset = vec3( mod( gl_InstanceID, 50 ) * 0.03f, -gl_InstanceID / 50 * 0.03f, 0.0 );
    gl_Position =  vec4(position + offset, 1.0) + instance_position;
	
	fragment.color = instance_color;
}
