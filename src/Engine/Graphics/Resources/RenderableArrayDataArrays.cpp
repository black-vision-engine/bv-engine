#include "RenderableArrayDataArrays.h"

#include <cassert>

#include "Engine\Graphics\Resources\VertexArray.h"

namespace bv
{

// *********************************
//
RenderableArrayDataArraysSingleVertexBuffer::RenderableArrayDataArraysSingleVertexBuffer        ( VertexBuffer * vb, VertexDescriptor * vd )
    : RenderableArrayDataSingleVertexBuffer( RenderableArrayDataEnumKind::RADEK_VAO, vb, vd )
    , m_vao( nullptr )
{
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
unsigned int             RenderableArrayDataArraysSingleVertexBuffer::GetNumConnectedComponents () const
{
    return m_vao->GetNumConnectedComponents();
}

// *********************************
//
unsigned int             RenderableArrayDataArraysSingleVertexBuffer::GetNumVerticesInConnectedComponent ( unsigned int ccNum ) const
{
    return m_vao->GetNumVertices( ccNum );
}

}
