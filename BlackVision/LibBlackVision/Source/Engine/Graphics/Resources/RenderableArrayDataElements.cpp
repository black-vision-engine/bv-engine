#include "stdafx.h"

#include "RenderableArrayDataElements.h"

#include <cassert>


namespace bv
{

// *********************************
//
RenderableArrayDataElements::RenderableArrayDataElements ( VertexBuffer * vb, VertexDescriptor * vd )
    : RenderableArrayDataSingleVertexBuffer( RenderableArrayDataEnumKind::RADEK_ELEMENTS, vb, vd )
{
}

// *********************************
//
const IndexBuffer *     RenderableArrayDataElements::GetIndexBuffer( int ccNum ) const
{
    assert( ccNum >= 0 );
    assert( ccNum < (int) m_inedxBuffersVec.size() );

    return m_inedxBuffersVec[ ccNum ];
}

//FIXME: delete or uncomment if useful
//// *********************************
////
//unsigned int                    RenderableArrayDataSingleVertexBuffer::GetCCOffset                 ( int ccNum ) const
//{
//    assert( ccNum >= 0 );
//    assert( ccNum < (int) m_ccOffsets.size() );
//
//    return m_ccOffsets[ ccNum ];
//}
//
//// *********************************
////
//unsigned int                    RenderableArrayDataSingleVertexBuffer::GetCCNumVertices            ( int ccNum ) const
//{
//    assert( ccNum >= 0 );
//    assert( ccNum < (int) m_numVerticesInCC.size() );
//
//    return m_numVerticesInCC[ ccNum ];
//}

// *********************************
//
SizeType                    RenderableArrayDataElements::GetNumConnectedComponents   () const
{
//    assert( m_numVerticesInCC.size() == m_ccOffsets.size() );
//    return m_numVerticesInCC.size();

    return m_inedxBuffersVec.size();
}

// *********************************
//
unsigned int                RenderableArrayDataElements::GetNumVerticesInConnectedComponent  ( unsigned int ccNum ) const
{
    { ccNum; } // FIXME: suppress unused warning
    assert( false ); //FIXME: implement

    return 0;
}

}
