#include "RenderableArrayData.h"

#include <cassert>

#include "Engine\Graphics\Resources\VertexBuffer.h"
#include "Engine\Graphics\Resources\VertexDescriptor.h"

namespace bv
{

// *********************************
//
RenderableArrayData::RenderableArrayData         ( RenderableArrayDataEnumKind type )
    : m_type( type )
{
}

// *********************************
//
RenderableArrayData::~RenderableArrayData        ()
{
}

// *********************************
//
RenderableArrayDataEnumKind     RenderableArrayData::Type                        () const
{
    return m_type;
}

// *********************************
//
const VertexDescriptor *        RenderableArrayData::GetVertexDecscriptor        () const
{
    return m_vertexDescriptor;
}

// *********************************
//
const VertexBuffer *            RenderableArrayData::GetVertexBuffer             () const
{
    return m_vertexBuffer;
}

// *********************************
//
unsigned int                    RenderableArrayData::GetCCOffset                 ( int ccNum ) const
{
    assert( ccNum >= 0 );
    assert( ccNum < (int) m_ccOffsets.size() );

    return m_ccOffsets[ ccNum ];
}

// *********************************
//
unsigned int                    RenderableArrayData::GetCCNumVertices            ( int ccNum ) const
{
    assert( ccNum >= 0 );
    assert( ccNum < (int) m_numVerticesInCC.size() );

    return m_numVerticesInCC[ ccNum ];
}

// *********************************
//
unsigned int                    RenderableArrayData::GetNumConnectedComponents   () const
{
    assert( m_numVerticesInCC.size() == m_ccOffsets.size() );

    return m_ccOffsets.size();
}

}
