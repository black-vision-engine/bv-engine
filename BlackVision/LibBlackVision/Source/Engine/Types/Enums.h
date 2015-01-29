#pragma once

namespace bv
{

// NOTE: used by TextureBase::m_sPixelSize (TextureBase.cpp)
// NOTE: used by GLConstantTextureFormat and GLConstantTextureType (PdrConstants.cpp)
enum class TextureFormat : int
{
    F_A8R8G8B8 = 0  ,
    F_R8G8B8        ,
    F_A8            ,
    F_L8            ,
    F_R32F          ,
    F_TOTAL
};

enum class TextureType : int
{
    T_2D = 0        ,
    T_CUBE          ,

    T_TOTAL
};

//FIXME: there so many dependencies on this type that it they all should be kept in one place (adding PT_MAT2 required a shitload of changes in the code)
enum class ParamType : int
{
    PT_FLOAT1 = 0,
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

enum class ModelParamType : int
{
    MPT_FLOAT = 0,
    MPT_MAT2,
    MPT_VEC2,
    MPT_VEC3,
    MPT_VEC4,
    MPT_TRANSFORM,
    MPT_TRANSFORM_VEC,
    MPT_TOTAL
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

enum class TextureAttachmentMode : int
{
    MM_ATTACHED = 0,
    MM_FREE,

    MM_TOTAL
};

enum class TextureWrappingMode : int
{
    TWM_CLAMP = 0,
    TWM_REPEAT,
    TWM_MIRROR,
    TWM_CLAMP_BORDER,
    TWM_CLAMP_EDGE, 

    TWM_TOTAL
};

enum class TextureFilteringMode : int
{
    TFM_POINT = 0,
    TFM_LINEAR,

    TFM_TOTAL
};

enum class PluginResourceType : int
{
    PRT_TEXTURE = 0,
    PRT_ANIMATION,
    PRT_FONT,

    PRT_TOTAL
};

enum class TimelineEventType : int
{
    TET_STOP = 0,
    TET_LOOP,
    TET_NULL,

    TET_TOTAL
};

enum class TimelinePlayDirection : int
{
    TPD_FORWAD = 0,
    TPD_BACKWARD,

    TPD_TOTAL
};

enum class TimelineWrapMethod : int
{
    TWM_CLAMP = 0,
    TWM_REPEAT,
    TWM_MIRROR,

    TWM_TOTAL
};

enum class LoopEventAction : int
{
    LEA_REVERSE = 0,
    LEA_RESTART,
    LEA_GOTO,

    LEA_TOTAL
};

enum class MipMapFilterType : int
{
	BOX = 0,
	BILINEAR,
	B_SPLINE,
	BICUBIC,
	CATMULL_ROM,
	LANCZOS
};

} // bv