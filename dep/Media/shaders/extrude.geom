#version 400

layout ( triangles ) in;

layout ( triangle_strip, max_vertices = 16 ) out;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 P;
uniform float scale;

out float depth;

mat4 dummy()
{
    return P;
}

vec2 GetMinMaxZ()
{
    float posMin =  (MV * gl_in[0].gl_Position).z;        // 1

    float posMax =  posMin;        // 1

    posMin = min(posMin , (MV * gl_in[1].gl_Position).z);         // 2
    posMax = max(posMax , (MV * gl_in[1].gl_Position).z);         // 2
                             
    posMin = min(posMin , (MV * gl_in[2].gl_Position).z);         // 2
    posMax = max(posMax , (MV * gl_in[2].gl_Position).z);         // 2
                             
    posMin = min(posMin , (MV * (gl_in[1].gl_Position + vec4(0.0, 0.0, -1.0 , 0.0))).z);         // 2
    posMax = max(posMax , (MV * (gl_in[1].gl_Position + vec4(0.0, 0.0, -1.0 , 0.0))).z);         // 2
                             
    posMin = min(posMin , (MV * (gl_in[2].gl_Position + vec4(0.0, 0.0, -1.0 * scale, 0.0))).z);         // 2
    posMax = max(posMax , (MV * (gl_in[2].gl_Position + vec4(0.0, 0.0, -1.0 * scale, 0.0))).z);         // 2
                             
    posMin = min(posMin , (MV * (gl_in[2].gl_Position + vec4(1.0, 0.0, 0.0, 0.0))).z);         // 2
    posMax = max(posMax , (MV * (gl_in[2].gl_Position + vec4(1.0, 0.0, 0.0, 0.0))).z);         // 2
                             
    posMin = min(posMin , (MV * (gl_in[2].gl_Position + vec4(1.0, 0.0, -1.0 * scale, 0.0))).z);         // 2
    posMax = max(posMax , (MV * (gl_in[2].gl_Position + vec4(1.0, 0.0, -1.0 * scale, 0.0))).z);         // 2
                             
    posMin = min(posMin , (MV * (gl_in[0].gl_Position + vec4(0.0, 0.0, -1.0 * scale, 0.0))).z);         // 2
    posMax = max(posMax , (MV * (gl_in[0].gl_Position + vec4(0.0, 0.0, -1.0 * scale, 0.0))).z);         // 2

	return vec2( min( gl_in[0].gl_Position.z, -1.0 * scale ), max( gl_in[0].gl_Position.z, -1.0 * scale ) );
    //return vec2(posMin, posMax);
}

void CalcDepth(vec2 minMax, float z)
{
    depth = (z - minMax.x) / (minMax.y - minMax.x);
}

void MyEmitVertex(int i, vec2 pos, vec4 offset)
{
    gl_Position = MVP * (gl_in[i].gl_Position + offset);        // 1
    //CalcDepth(pos, (MV * (gl_in[i].gl_Position + offset)).z);
    CalcDepth(pos, ((gl_in[i].gl_Position + offset)).z);
    EmitVertex();
}

void main()
{
    dummy();

    vec2 pos = GetMinMaxZ();

    MyEmitVertex(0, pos, vec4(0.0, 0.0, 0.0, 0.0));
    MyEmitVertex(1, pos , vec4(0.0, 0.0, 0.0, 0.0));
    MyEmitVertex(2, pos , vec4(0.0, 0.0, 0.0, 0.0));
    MyEmitVertex(1, pos , vec4(0.0, 0.0, -1.0 * scale, 0.0));
    MyEmitVertex(2, pos , vec4(0.0, 0.0, -1.0 * scale, 0.0));
    MyEmitVertex(0, pos , vec4(0.0, 0.0, -1.0 * scale, 0.0));
    MyEmitVertex(2, pos , vec4(0.0, 0.0, 0.0, 0.0));
    MyEmitVertex(0, pos , vec4(0.0, 0.0, 0.0, 0.0));
    
    EndPrimitive();

    MyEmitVertex(0 , pos, vec4(0.0, 0.0, 0.0, 0.0));
    MyEmitVertex(0 , pos, vec4(0.0, 0.0, -1.0* scale, 0.0));
    MyEmitVertex(1 , pos, vec4(0.0, 0.0, 0.0, 0.0));
    MyEmitVertex(1 , pos, vec4(0.0, 0.0, -1.0* scale, 0.0));
    
    EndPrimitive();
 
    
}