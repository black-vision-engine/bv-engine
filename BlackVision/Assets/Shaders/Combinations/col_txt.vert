#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;
layout (location = 2) in vec2 ccCenter;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;

uniform int     cc_num;
uniform int     cc_num_total;

uniform float   time;
uniform vec2    explosionCenter;

out vec2 uvCoord;

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

mat4 translateMatrix( vec3 transVec )
{
    return mat4(1.0,  0.0,  0.0,  0.0,
                0.0,  1.0,  0.0,  0.0,
                0.0,  0.0,  1.0,  0.0,
                transVec.x,  transVec.y,  transVec.z,  1.0);
}

void main()
{
    vec2 ccc = ccCenter;
    //mat4 rot = translateMatrix( vec3( ccc.x, ccc.y, 0.0 ) ) * rotationMatrix( vec3( 0.0 , 0.0 , 1.0 ), time * cc_num * 0.1 ) * translateMatrix( vec3( -ccc.x, -ccc.y, 0.0 ) );

    vec2 expCenterDirection = ccc - explosionCenter;
    
    float expCenterDist = length( expCenterDirection );
    
    vec2 expCenterDirectionNorm = normalize( expCenterDirection );
    
    mat4 explodeMatrix = translateMatrix( vec3( expCenterDirectionNorm.x * expCenterDist, expCenterDirectionNorm.y * expCenterDist, 0.0 ) ) ;
    
    
    //gl_Position = rot * vec4( vertexPosition, 1.0 );
    gl_Position = explodeMatrix * vec4( vertexPosition, 1.0 );
    uvCoord = ( vec4( vertexTexCoord, 0.0, 1.0 ) ).xy;
}
