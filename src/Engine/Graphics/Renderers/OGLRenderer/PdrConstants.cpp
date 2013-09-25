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

} //anonymous

// ****************************
//
GLuint ConstantsMapper::GlConstant( MemoryLockingType mlt )
{
    return BufferLockingGL[ (int) mlt ]; 
}

// ****************************
//
GLuint ConstantsMapper::GlConstant( DataBuffer::Semantic semantic )
{
    return BufferUsageGL[ (int) semantic ];     
}

// ****************************
//
GLuint ConstantsMapper::GlConstant( RenderableEntity::RenderableType type )
{
    return RenderableTypeGL[ (int) type ];     
}

// ****************************
//
GLuint ConstantsMapper::GlConstant( FaceKind face )
{
    return FaceCullingTypeGL[ (int) face ];
}

// ****************************
//
GLuint ConstantsMapper::GlConstant( SamplerWrappingMode wrappingMode )
{
    return WrappingModeGL[ (int) wrappingMode ];
}

// ****************************
//
GLuint ConstantsMapper::GlConstant( SamplerFilteringMode filteringMode )
{
    return FilteringModeGL[ (int) filteringMode ];
}

}
