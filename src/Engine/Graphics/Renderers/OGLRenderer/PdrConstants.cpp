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
}

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

}
