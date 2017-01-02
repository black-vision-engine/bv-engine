#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D Texture;

void main()
{
	vec4 col = vec4(0.0, 0.0, 0.0, 1.0);

	if( uvCoord.y > 0.5 )
	{
		float u = 2.0 * uvCoord.x;
		vec2 uv = vec2(u - floor(u), 2.0 * uvCoord.y - 1.0);
		
		col = texture( Texture, uv );
		
		if ( u >= 1.0 )
		{
			col = col.aaaa;
		}
	}

    FragColor = col.rgba;
}
