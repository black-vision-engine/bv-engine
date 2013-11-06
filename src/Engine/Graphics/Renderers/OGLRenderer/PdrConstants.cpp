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
        GL_STATIC_DRAW,     
        GL_DYNAMIC_DRAW,    
        GL_DYNAMIC_DRAW,    
        GL_DYNAMIC_DRAW,    
        GL_DYNAMIC_DRAW     
    };

    GLuint RenderableTypeGL[] =
    {
        GL_TRIANGLES,
        GL_TRIANGLE_STRIP
    };

    GLenum FaceCullingTypeGL[] =
    {
        GL_FRONT,
        GL_BACK,
        GL_FRONT_AND_BACK,
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
        GL_ONE_MINUS_CONSTANT_ALPHA //ASBM_ONE_MINUS_CONSTANT_ALPHA,
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
        GL_ONE_MINUS_CONSTANT_ALPHA     //ADBM_ONE_MINUS_CONSTANT_ALPHA,      
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
        GL_ALWAYS       //ACM_ALWAYS,
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
GLuint ConstantsMapper::GLConstant( FaceKind face )
{
    return FaceCullingTypeGL[ (int) face ];
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
GLuint ConstantsMapper::GLConstant( AlphaCompareMode mode )
{
    return AlphaCompareModeGL[ (int) mode ];
}

}
