#include "stdafx.h"

#include "PdrUniformBufferObject.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Resources/UniformBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"


namespace bv {


// *******************************
//
PdrUniformBufferObject::PdrUniformBufferObject     ( Renderer * renderer, const UniformBuffer * ub, UInt32 blockBindingIdx, const std::string & blockName )
    : m_blockBindingIdx( blockBindingIdx )
    , m_blockName( blockName )
{
    { renderer; } // FIXME: suppress unused
    CreateBuffer( ub );
}

// *******************************
//
PdrUniformBufferObject::~PdrUniformBufferObject    ()
{
    BVGL::bvglDeleteBuffers( 1, &m_bufferHandle );
}

// *******************************
//
void PdrUniformBufferObject::Enable         ( Renderer * renderer )
{
    { renderer; } // FIXME: suppress unused
    Bind();
}

// *******************************
//
void PdrUniformBufferObject::Disable        ( Renderer * renderer )
{
    { renderer; } // FIXME: suppress unused
    Unbind();
}

// *******************************
//
void * PdrUniformBufferObject::Lock         ( MemoryLockingType mlt )
{
    Bind();

    GLvoid * vidMem = BVGL::bvglMapBuffer( GL_UNIFORM_BUFFER, ConstantsMapper::GLConstant( mlt ) );
    
    Unbind();

    return vidMem;
}

// *******************************
//
void PdrUniformBufferObject::Unlock         ()
{
    Bind();

    BVGL::bvglUnmapBuffer( GL_UNIFORM_BUFFER );

    Unbind();
}

// *******************************
//
void    PdrUniformBufferObject::Update              ( const UniformBuffer * ub )
{
    void * data = Lock( MemoryLockingType::MLT_WRITE_ONLY );
    memcpy( data, ub->Data(), ub->Size() );
    Unlock();
}

// *******************************
//
void    PdrUniformBufferObject::Recreate            ( const UniformBuffer * ub )
{
    BVGL::bvglDeleteBuffers( 1, &m_bufferHandle );
    CreateBuffer( ub );
}

// *******************************
//
void    PdrUniformBufferObject::Bind                ()
{
    BVGL::bvglBindBuffer( GL_UNIFORM_BUFFER, m_bufferHandle );
}

// *******************************
//
void    PdrUniformBufferObject::Unbind              ()
{
    BVGL::bvglBindBuffer( GL_UNIFORM_BUFFER, 0 );
}

// *******************************
//
void    PdrUniformBufferObject::UniformBlockBinding ( UInt32 program )
{
    //GLuint program;
    //BVGL::bvglGetIntegerv( GL_CURRENT_PROGRAM, ( GLint * )&program );
    auto blockIdx = BVGL::bvglGetUniformBlockIndex( program, m_blockName.c_str() );
    BVGL::bvglUniformBlockBinding( program, blockIdx, m_blockBindingIdx );
}

// *******************************
//
void    PdrUniformBufferObject::BufferData          ( const UniformBuffer * ub ) const
{
    BVGL::bvglBufferData( GL_UNIFORM_BUFFER, ub->Size(), 0, ConstantsMapper::GLConstant( ub->GetSemantic() ) );
}

// *******************************
//
void    PdrUniformBufferObject::CreateBuffer        ( const UniformBuffer * ub )
{
    BVGL::bvglGenBuffers( 1, &m_bufferHandle );

    Bind();
    BufferData( ub );
    Unbind();

    void * data = Lock( MemoryLockingType::MLT_WRITE_ONLY );
    memcpy( data, ub->Data(), ub->Size() );

    Unlock();
 
    BVGL::bvglBindBufferBase( GL_UNIFORM_BUFFER, m_blockBindingIdx, m_bufferHandle );
}

}
