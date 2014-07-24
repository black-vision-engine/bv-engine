#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D HeightMapTex;

uniform float alpha;
uniform float scaleX;

float decodeHeight( vec4 col )
{
    return float( 256.0 * 255.0 * col.r + 255.0 * col.g ) / 16.0;
}

int insideHillTest( vec2 point )
{
	vec4 col = texture( HeightMapTex, point );
    float h = decodeHeight( col ) / 1009.1532; //MAX_VAL
    
    float off_y = point.y - 0.17;//OFFSET
    if( off_y < 0.0 )
        return -1;

    if( h > off_y * 3.5f ) //SCALE_VAL, OFFSET_VAL
        return 1;

    return 0;
}

vec4 yebaneSuoneczko( vec2 pos )
{
    float dx = ( pos.x - 0.8 ) * scaleX;
    float dy = ( pos.y - 0.8 ) * 1080.0 / 1920.0;
    
    float dist = sqrt( dx * dx + dy * dy );

    vec4 inner_col = vec4( 1.0, 1.0, 0.0, 1.0 );
    vec4 outer_col = vec4( 0.6, 0.2, 1.0, 0.5 );

    return mix( inner_col, outer_col, smoothstep( 0.07, 0.12, dist ) );
}


void main()
{
    int ih = insideHillTest( uvCoord );

    vec4 col = vec4( 1.0, 1.0, 1.0, 1.0 );

    if( ih == -1 )
        col = vec4( 0.0, 0.5, 1.0, 0.7 );
    else if( ih == 0 )
        col = yebaneSuoneczko( uvCoord );
    
    FragColor = col;
    //float h = isInsideHill( uvCoord );
    //FragColor = vec4( col.rgb, 1.0 );
}
