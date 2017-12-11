#include "stdafx.h"

#include "PdrUniformBufferObject.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

#include <algorithm>


#include "Memory/MemoryLeaks.h"



namespace bv {


// *******************************
//
PdrUniformBufferObject::PdrUniformBufferObject     ( Renderer * renderer, const UniformBuffer * ub, UInt32 blockBindingIdx )
    : m_blockBindingIdx( blockBindingIdx )
{
    m_blockName = ub->GetLayout()->blockName;
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
void                PdrUniformBufferObject::Enable         ( Renderer * renderer )
{
    { renderer; } // FIXME: suppress unused
    Bind();
}

// *******************************
//
void                PdrUniformBufferObject::Disable        ( Renderer * renderer )
{
    { renderer; } // FIXME: suppress unused
    Unbind();
}

// *******************************
//
void *              PdrUniformBufferObject::Lock            ( MemoryLockingType mlt )
{
    Bind();

    GLvoid * vidMem = BVGL::bvglMapBuffer( GL_UNIFORM_BUFFER, ConstantsMapper::GLConstant( mlt ) );
    
    Unbind();

    return vidMem;
}

// *******************************
//
void                PdrUniformBufferObject::Unlock         ()
{
    Bind();

    BVGL::bvglUnmapBuffer( GL_UNIFORM_BUFFER );

    Unbind();
}

// *******************************
//
void                PdrUniformBufferObject::Update          ( const UniformBuffer * ub )
{
    //void * data = Lock( MemoryLockingType::MLT_WRITE_ONLY );
    //memcpy( data, ub->GetData(), ub->Size() );
    //Unlock();

    Bind();

    BVGL::bvglBufferSubData( GL_UNIFORM_BUFFER, 0, ub->Size(), ub->GetData() );

    Unbind();
}

// *******************************
//
void                PdrUniformBufferObject::Recreate        ( const UniformBuffer * ub )
{
    BVGL::bvglDeleteBuffers( 1, &m_bufferHandle );
    CreateBuffer( ub );
}

// *******************************
//
void                PdrUniformBufferObject::Bind            ()
{
    BVGL::bvglBindBuffer( GL_UNIFORM_BUFFER, m_bufferHandle );
}

// *******************************
//
void                PdrUniformBufferObject::Unbind          ()
{
    BVGL::bvglBindBuffer( GL_UNIFORM_BUFFER, 0 );
}

// *******************************
//
void                PdrUniformBufferObject::UniformBlockBinding     ( UInt32 program ) const
{
    auto blockIdx = BVGL::bvglGetUniformBlockIndex( program, m_blockName.c_str() );
    BVGL::bvglUniformBlockBinding( program, blockIdx, m_blockBindingIdx );
}

// *******************************
//
void                PdrUniformBufferObject::BufferData      ( const UniformBuffer * ub ) const
{
    BVGL::bvglBufferData( GL_UNIFORM_BUFFER, ub->Size(), 0, ConstantsMapper::GLConstant( ub->GetSemantic() ) );
}

// *******************************
//
void                PdrUniformBufferObject::CreateBuffer    ( const UniformBuffer * ub )
{
    BVGL::bvglGenBuffers( 1, &m_bufferHandle );

    Bind();
    BufferData( ub );
    Unbind();

    void * data = Lock( MemoryLockingType::MLT_WRITE_ONLY );
    memcpy( data, ub->GetData(), ub->Size() );

    Unlock();
 
    BVGL::bvglBindBufferBase( GL_UNIFORM_BUFFER, m_blockBindingIdx, m_bufferHandle );
}

// *******************************
//
UniformBlockLayout *  PdrUniformBufferObject::GetUniformBlockLayout   ( UInt32 program, const std::string & blockName )
{
    GLuint blockIdx = BVGL::bvglGetUniformBlockIndex( program, blockName.c_str() );
    if( blockIdx == GL_INVALID_INDEX ) 
    {
        return nullptr;
    }
    
    auto layout = new UniformBlockLayout();

    layout->blockName = blockName;

    Int32 uboSize;
    BVGL::bvglGetActiveUniformBlockiv( program, blockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize );
    layout->size = uboSize;
    
    Int32 currBlock;
    BVGL::bvglGetActiveUniformBlockiv( program, blockIdx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &currBlock );
    
    auto indices = new UInt32[ currBlock ];
    BVGL::bvglGetActiveUniformBlockiv( program, blockIdx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, ( Int32 * )indices );
    
    Int32 len, offset;
	char name[ 256 ];
    
    for ( Int32 k = 0; k < currBlock; ++k )
    {
		BVGL::bvglGetActiveUniformName( program, indices[ k ], 256, &len, name );
        BVGL::bvglGetActiveUniformsiv( program, 1, &indices[ k ], GL_UNIFORM_OFFSET, &offset );
        
        UniformDesc desc;
        desc.name = name;
        desc.offset = offset;
        layout->uniformDescs.push_back( desc );
    }

    std::sort( layout->uniformDescs.begin(), layout->uniformDescs.end() );

    delete indices;
    return layout;
}

}
