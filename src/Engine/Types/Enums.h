#pragma once

namespace bv
{

enum class ParamType : int
{
    PT_FLOAT = 0,
    PT_FLOAT1,
    PT_FLOAT2,
    PT_FLOAT3,
    PT_FLOAT4,
    PT_MAT3,
    PT_MAT4,
    PT_INT,
    PT_BOOL,
    PT_TOTAL
};

enum class ParameterSemantic : int
{
    NONE,
    COLOR,
    LOCAL_TRANSFORM,
};

enum AttrType
{
    AT_NONE,
    AT_FLOAT1,
    AT_FLOAT2,
    AT_FLOAT3,
    AT_FLOAT4,
    AT_TOTAL
};

enum AttrSemantic
{
    AS_NONE,
    AS_POSITION,      
    AS_NORMAL,        
    AS_TEXCOORD,      
    AS_COLOR,
    AS_TOTAL
};

enum class PrimitiveType : int
{
    PT_POINTS = 0,
    PT_LINES,
    PT_LINE_STRIP,
    PT_LINE_LOOP,
    PT_TRIANGLES,
    PT_TRIANGLE_STRIP,
    PT_TRIANGLE_FAN,
    PT_TRIANGLE_MESH,
    PT_QUADS,
    PT_QUAD_STRIP,
    PT_QUAD_MESH,
    PT_POLYGON,

    PT_TOTAL,
};

} // bv
