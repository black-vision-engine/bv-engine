#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;
layout (location = 2) in vec2 ccCenter;
layout (location = 3) in vec2 vertexAlphaTexCoord;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;

uniform mat4 txAlphaMat;

uniform int         numTextLayers;

uniform int     cc_num;
uniform int     cc_num_total;

uniform float   time;

uniform float   transformEffectVal1;
uniform float   transformEffectVal2;

uniform float   animScale;
uniform float   animScaleOffset;

uniform vec2    explosionCenter;
uniform int     transformTextEffectId;

out vec2 uvCoord;
out vec2 ccCenterCoord;
out vec2 uvAlphaCoord;

int pseudoRandonInt( int seed, int total )  // Linear congruential generator
{   
    return ( 4 * seed ) % ( total + 1 );
}

float rand( float co ){
    return fract(sin(dot(vec2( co, co ) ,vec2(12.9898,78.233))) * 43758.5453);
}

float linearInterpolation( float begin, float end, float t )
{
    return begin * ( 1.0 - t ) + end * t;
}

mat4 scaleMatrix( vec3 scale )
{    
    return mat4(scale.x,    0.0,        0.0,        0.0,
                0.0,        scale.y,    0.0,        0.0,
                0.0,        0.0,        scale.z,    0.0,
                0.0,        0.0,        0.0,        1.0);
}

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

mat4 linearRotationCC( float begin, float end, int i, int total )
{
    float angle = linearInterpolation( begin, end, float( i % total ) / float( total ) );
    
    return rotationMatrix( vec3( 0.0 , 0.0 , 1.0 ), angle );
}

mat4 linearScaleCC( float begin, float end, int i, int total )
{
    float scale = linearInterpolation( begin, end, float( i ) / float( total ) );
    
    return scaleMatrix( vec3( 1.f, scale, 1.0 ) );
}

mat4 randomScaleCC( float begin, float end, int i, int total )
{    
    float t = rand( float( i ) / float( total ) );
    
    return scaleMatrix( vec3( 1.f, linearInterpolation( begin, end, t ), 1.0 ) );
}

mat4 translateMatrix( vec3 transVec )
{
    return mat4(1.0,  0.0,  0.0,  0.0,
                0.0,  1.0,  0.0,  0.0,
                0.0,  0.0,  1.0,  0.0,
                transVec.x,  transVec.y,  transVec.z,  1.0);
}

mat4 animLetterTransform()
{
	int ccPerLayer = ( cc_num_total + 1 ) / numTextLayers;
    float lf  = animScaleOffset * float( ccPerLayer );
    int l = int( floor( lf ) );
    
    if( l < 0 )
    {
        l = -l;
    }
    
    if( lf < 0.0 )
    {
        lf = -lf;
    }
    
	int letter_id = cc_id % ccPerLayer;
	
    if( letter_id == l )
    {
        float so = lf - float( l );
        float realScale = so + animScale * ( 1.0 - so );
        
        return scaleMatrix( vec3( realScale, realScale, 1.0 ) );
    }
    else if( letter_id > l ) 
    {
        return scaleMatrix( vec3( animScale, animScale, 1.0 ) );        
    }
    else if( letter_id < l )
    {
        return scaleMatrix( vec3( 1.0, 1.0, 1.0 ) );
    }
}

void main()
{
    mat4 transform = mat4( 1.0 );
    
    switch( transformTextEffectId )
    {
    case 1:
        transform = transform * translateMatrix( vec3( ccCenter.x, ccCenter.y, 0.0 ) ) * linearRotationCC( transformEffectVal1, transformEffectVal2, cc_num, cc_num_total ) * translateMatrix( vec3( -ccCenter.x, -ccCenter.y, 0.0 ) );
         break;
    case 2:
        transform = transform *translateMatrix( vec3( ccCenter.x, ccCenter.y, 0.0 ) ) * linearScaleCC( transformEffectVal1, transformEffectVal2, cc_num, cc_num_total ) * translateMatrix( vec3( -ccCenter.x, -ccCenter.y, 0.0 ) );
         break;     
    case 3:
        transform = transform * translateMatrix( vec3( ccCenter.x, ccCenter.y, 0.0 ) ) * randomScaleCC( transformEffectVal1, transformEffectVal2, cc_num, cc_num_total ) * translateMatrix( vec3( -ccCenter.x, -ccCenter.y, 0.0 ) );
         break;
    case 4:
        transform = transform * translateMatrix( vec3( ccCenter.x, ccCenter.y, 0.0 ) ) * animLetterTransform() * translateMatrix( vec3( -ccCenter.x, -ccCenter.y, 0.0 ) );
         break;

    default:
        break;
    }
    
    transform = MVP * transform;
    
    gl_Position = transform * vec4( vertexPosition, 1.0 );
    uvCoord = ( vec4( vertexTexCoord, 0.0, 1.0 ) ).xy;
    ccCenterCoord = ccCenter;
	uvAlphaCoord = ( txAlphaMat * vec4( vertexAlphaTexCoord, 0.0, 1.0 ) ).xy;
}

