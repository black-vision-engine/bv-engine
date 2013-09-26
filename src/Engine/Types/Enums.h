#pragma once

namespace bv
{

//FIXME: there so many dependencies on this type that it they all should be kept in one place (adding PT_MAT2 required a shitload of changes in the code)
enum class ParamType : int
{
    PT_FLOAT = 0,
    PT_FLOAT1,
    PT_FLOAT2,
    PT_FLOAT3,
    PT_FLOAT4,
    PT_MAT2,
    PT_MAT3,
    PT_MAT4,
    PT_INT,
    PT_BOOL,
    PT_TOTAL
};

//FIXME: TOTOTOTO KURWA TRZEBA WYPIERDOLIC I POCZYSCIC KOD Z DEPRECATED KODEN BASEN FAK
enum class ParameterSemantic : int
{
    NONE,
    COLOR,
    LOCAL_TRANSFORM,
};

enum class AttributeType : int
{
    AT_FLOAT1 = 0,
    AT_FLOAT2,
    AT_FLOAT3,
    AT_FLOAT4,

    AT_TOTAL
};

enum class AttributeSemantic : int
{
    AS_POSITION = 0,      
    AS_TEXCOORD,
    AS_COLOR,
    AS_NORMAL,        
    AS_BINORMAL,        
    AS_CUSTOM,

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

enum class ChannelRole : int
{
    CR_GENERATOR = 0,
    CR_PROCESSOR,
    CR_PASSTHROUGH,

    CR_TOTAL,

    CR_7 //Krisztjano Rezinaldo
};

enum class IndexType : int
{
    IT_UBYTE = 0,
    IT_USHORT,
    IT_UINT,

    IT_TOTAL
};

} // bv
