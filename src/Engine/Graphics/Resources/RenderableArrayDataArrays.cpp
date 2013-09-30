#include "RenderableArrayDataArrays.h"

#include <cassert>

#include "Engine\Graphics\Resources\VertexArray.h"

namespace bv
{

// *********************************
//
RenderableArrayDataArraysSingleVertexBuffer::RenderableArrayDataArraysSingleVertexBuffer        ( VertexBuffer * vb, VertexDescriptor * vd )
    : RenderableArrayDataSingleVertexBuffer( RenderableArrayDataEnumKind::RADEK_VAO, vb, vd )
{
}

// *********************************
//
RenderableArrayDataArraysSingleVertexBuffer::~RenderableArrayDataArraysSingleVertexBuffer       ()
{
    //FIXME: for now let's assume that this class owns all added vaos

    for( auto vao : m_vaoVec )
    {
        delete vao;
    }
}

// *********************************
//
const VertexArraySingleVertexBuffer *   RenderableArrayDataArraysSingleVertexBuffer::VAO        ( unsigned int ccNum ) const
{
    assert( ccNum >= 0 );
    assert( ccNum < m_vaoVec.size() );

    return m_vaoVec[ ccNum ];
}

// *********************************
//
void                     RenderableArrayDataArraysSingleVertexBuffer::AddVAO                    ( VertexArraySingleVertexBuffer * vao )
{
    m_vaoVec.push_back( vao );
}

// *********************************
//
unsigned int             RenderableArrayDataArraysSingleVertexBuffer::GetNumConnectedComponents () const
{
    return m_vaoVec.size();
}

// *********************************
//
unsigned int             RenderableArrayDataArraysSingleVertexBuffer::GetNumVerticesInConnectedComponent ( unsigned int ccNum ) const
{
    assert( ccNum < m_vaoVec.size() );

    return m_vaoVec[ ccNum ]->GetNumVertices();
}

}
