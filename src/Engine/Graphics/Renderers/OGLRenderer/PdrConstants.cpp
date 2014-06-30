#include "PdrConstants.h"

namespace bv {

namespace
{
    GLuint BufferLockingGL[] =
    {
        GL_READ_ONLY,   
        GL_WRITE_ONLY,  
        GL_READ_WRITE   
    };

    GLuint BufferUsageGL[] =
    {
        GL_STATIC_DRAW,     // S_STATIC,  
        GL_DYNAMIC_DRAW,    // S_DYNAMIC
        GL_DYNAMIC_DRAW,    // S_RENDERTARGET
        GL_DYNAMIC_COPY,    // S_RENDERTARGET_STREAMING_READ
        GL_DYNAMIC_DRAW,    // S_DEPTHSTENCIL
        GL_DYNAMIC_DRAW,    // S_TEXTURE
        GL_STREAM_READ,     // S_TEXTURE_STREAMING_READ
        GL_STREAM_DRAW      // S_TEXTURE_STREAMING_WRITE
    };                    

    GLuint RenderableTypeGL[] =
    {
        GL_TRIANGLES,
        GL_TRIANGLE_STRIP
    };

    GLuint WrappingModeGL[] = 
    {
        GL_CLAMP,           //SWM_CLAMP,
        GL_REPEAT,          //SWM_REPEAT,
        GL_MIRRORED_REPEAT, //SWM_MIRROR,
        GL_CLAMP_TO_BORDER, //SWM_CLAMP_BORDER,
        GL_CLAMP_TO_EDGE    //SWM_CLAMP_EDGE, 
    };

    //FIXME: point_point, point_linear, linear_point, linear_linear -> add when mipmapping is more or less implemented 
    //GL_NEAREST_MIPMAP_NEAREST,GL_NEAREST_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_NEAREST,GL_LINEAR_MIPMAP_LINEAR
    GLuint FilteringModeGL[] = 
    {
        GL_NEAREST,   // SFM_POINT = 0,
        GL_LINEAR     // SFM_LINEAR
    };

    GLuint AlphaSrcBlendModeGL[] =
    {
        GL_ZERO,                    //ASBM_ZERO,
        GL_ONE,                     //ASBM_ONE,
        GL_DST_COLOR,               //ASBM_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR,     //ASBM_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA,               //ASBM_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,     //ASBM_ONE_MINUS_SRC_ALPHA,
        GL_DST_ALPHA,               //ASBM_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA,     //ASBM_ONE_MINUS_DST_ALPHA,
        GL_SRC_ALPHA_SATURATE,      //ASBM_SRC_ALPHA_SATURATE,
        GL_CONSTANT_COLOR,          //ASBM_CONSTANT_COLOR,
        GL_ONE_MINUS_CONSTANT_COLOR,//ASBM_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA,          //ASBM_CONSTANT_ALPHA,
        GL_ONE_MINUS_CONSTANT_ALPHA //ASBM_ONE_MINUS_CONSTANT_ALPHA
    };

    GLuint AlphaDstBlendModeGL[] =
    {
        GL_ZERO,                        //ADBM_ZERO,
        GL_ONE,                         //ADBM_ONE,
        GL_SRC_COLOR,                   //ADBM_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR,         //ADBM_ONE_MINUS_SRC_COLOR,
        GL_SRC_ALPHA,                   //ADBM_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,         //ADBM_ONE_MINUS_SRC_ALPHA,
        GL_DST_ALPHA,                   //ADBM_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA,         //ADBM_ONE_MINUS_DST_ALPHA,
        GL_CONSTANT_COLOR,              //ADBM_CONSTANT_COLOR,
        GL_ONE_MINUS_CONSTANT_COLOR,    //ADBM_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA,              //ADBM_CONSTANT_ALPHA,
        GL_ONE_MINUS_CONSTANT_ALPHA     //ADBM_ONE_MINUS_CONSTANT_ALPHA    
    };

    GLuint AlphaCompareModeGL[] =
    {
        GL_NEVER,       //ACM_NEVER = 0,
        GL_LESS,        //ACM_LESS,
        GL_EQUAL,       //ACM_EQUAL,
        GL_LEQUAL,      //ACM_LEQUAL,
        GL_GREATER,     //ACM_GREATER,
        GL_NOTEQUAL,    //ACM_NOTEQUAL,
        GL_GEQUAL,      //ACM_GEQUAL,
        GL_ALWAYS       //ACM_ALWAYS
    };

    GLuint DepthCompareModeGL[] =
    {
        GL_NEVER,       //DCM_NEVER = 0,
        GL_LESS,        //DCM_LESS,
        GL_EQUAL,       //DCM_EQUAL,
        GL_LEQUAL,      //DCM_LEQUAL,
        GL_GREATER,     //DCM_GREATER,
        GL_NOTEQUAL,    //DCM_NOTEQUAL,
        GL_GEQUAL,      //DCM_GEQUAL,
        GL_ALWAYS       //DCM_ALWAYS
    };

    GLuint FillStateModeGL[] =
    {
        GL_POINT, //FSM_POINTS = 0,
        GL_LINE, //FSM_LINES,
        GL_FILL //FSM_POLYGONS
    };

    GLuint TextureFormatGL[] =
    {
        GL_BGRA,        // F_A8R8G8B8
        GL_BGR,         // F_R8G8B8
        GL_ALPHA,       // F_A8
        GL_LUMINANCE,   // F_L8
        GL_R32F         // F_R32F
    };

    GLuint TextureInternalFormatGL[] =
    {
        GL_RGBA8,       // F_A8R8G8B8
        GL_RGB8,        // F_R8G8B8
        GL_ALPHA8,      // F_A8
        GL_LUMINANCE8,  // F_L8 
        GL_R32F         // F_R32F
    };

    GLuint TextureTypeGL[] = 
    {
        GL_UNSIGNED_BYTE,   // F_A8R8G8B8
        GL_UNSIGNED_BYTE,   // F_R8G8B8
        GL_UNSIGNED_BYTE,   // F_A8
        GL_UNSIGNED_BYTE,   // F_L8
        GL_FLOAT            // F_R32F
    };

} //anonymous

// ****************************
//
GLuint ConstantsMapper::GLConstant( MemoryLockingType mlt )
{
    return BufferLockingGL[ (int) mlt ]; 
}

// ****************************
//
GLuint ConstantsMapper::GLConstant( DataBuffer::Semantic semantic )
{
    return BufferUsageGL[ (int) semantic ];     
}

// ****************************
//
GLuint ConstantsMapper::GLConstant( RenderableEntity::RenderableType type )
{
    return RenderableTypeGL[ (int) type ];     
}

// ****************************
//
GLuint ConstantsMapper::GLConstant( SamplerWrappingMode wrappingMode )
{
    return WrappingModeGL[ (int) wrappingMode ];
}

// ****************************
//
GLuint ConstantsMapper::GLConstant( SamplerFilteringMode filteringMode )
{
    return FilteringModeGL[ (int) filteringMode ];
}

// ****************************
//
GLuint ConstantsMapper::GLConstant( AlphaSrcBlendMode mode )
{
    return AlphaSrcBlendModeGL[ (int) mode ];
}

// ****************************
//
GLuint ConstantsMapper::GLConstant( AlphaDstBlendMode mode )
{
    return AlphaDstBlendModeGL[ (int) mode ];
}

// ****************************
//
GLuint ConstantsMapper::GLConstant( DepthCompareMode mode )
{
    return DepthCompareModeGL[ (int) mode ];
}

// ****************************
//
GLuint ConstantsMapper::GLConstant( FillStateMode mode )
{
    return FillStateModeGL[ (int) mode ];
}

// ****************************
//
GLuint ConstantsMapper::GLConstantTextureFormat         ( TextureFormat fmt )
{
    return TextureFormatGL[ (int) fmt ];
}

// ****************************
//
GLuint ConstantsMapper::GLConstantTextureInternalFormat ( TextureFormat fmt )
{
    return TextureInternalFormatGL[ (int) fmt ];
}

// ****************************
//
GLuint ConstantsMapper::GLConstantTextureType           ( TextureFormat fmt )
{
    return TextureTypeGL[ (int) fmt ];
}

} //bv
