#include "RenderableArrayDataArrays.h"

#include <cassert>

#include "Engine/Graphics/Resources/VertexArray.h"

namespace bv
{

// *********************************
//
RenderableArrayDataArraysSingleVertexBuffer::RenderableArrayDataArraysSingleVertexBuffer        ( VertexArraySingleVertexBuffer * vao )
    : RenderableArrayDataSingleVertexBuffer( RenderableArrayDataEnumKind::RADEK_VAO, vao->GetVertexBuffer(), vao->GetVertexDescriptor() )
    , m_vao( vao )
{
    assert( vao != nullptr );
}

// *********************************
//
RenderableArrayDataArraysSingleVertexBuffer::~RenderableArrayDataArraysSingleVertexBuffer       ()
{
    //FIXME: for now let's assume that this class owns all added vaos
    delete m_vao;
}

// *********************************
//
const VertexArraySingleVertexBuffer *   RenderableArrayDataArraysSingleVertexBuffer::VAO        () const
{
    return m_vao;
}

// *********************************
//
VertexArraySingleVertexBuffer *         RenderableArrayDataArraysSingleVertexBuffer::VAO        ()
{
    return m_vao;
}

// *********************************
//
void                     RenderableArrayDataArraysSingleVertexBuffer::SetVAO                    ( VertexArraySingleVertexBuffer * vao )
{
    if ( m_vao != nullptr )
    {
        //FIXME: for now let's assume that this class owns all added vaos
        delete m_vao;
    }

    m_vao = vao;
}

// *********************************
//
SizeType                RenderableArrayDataArraysSingleVertexBuffer::GetNumConnectedComponents () const
{
    return m_vao->GetNumConnectedComponents();
}

// *********************************
//
unsigned int            RenderableArrayDataArraysSingleVertexBuffer::GetNumVerticesInConnectedComponent ( unsigned int ccNum ) const
{
    return m_vao->GetNumVertices( ccNum );
}

}
