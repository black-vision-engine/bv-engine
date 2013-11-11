#include "RenderableArrayData.h"

#include <cassert>

#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexDescriptor.h"

namespace bv
{

// *********************************
//
RenderableArrayDataSingleVertexBuffer::RenderableArrayDataSingleVertexBuffer                    ( RenderableArrayDataEnumKind type, VertexBuffer * vb, VertexDescriptor * vd )
    : m_type( type )
    , m_vertexBuffer( vb )
    , m_vertexDescriptor( vd )
{
}

// *********************************
//
RenderableArrayDataSingleVertexBuffer::~RenderableArrayDataSingleVertexBuffer                   ()
{
}

// *********************************
//
RenderableArrayDataEnumKind     RenderableArrayDataSingleVertexBuffer::Type                     () const
{
    return m_type;
}

// *********************************
//
const VertexDescriptor *        RenderableArrayDataSingleVertexBuffer::GetVertexDescriptor      () const
{
    return m_vertexDescriptor;
}

// *********************************
//
const VertexBuffer *            RenderableArrayDataSingleVertexBuffer::GetVertexBuffer          () const
{
    return m_vertexBuffer;
}


// *********************************
//
VertexDescriptor *        RenderableArrayDataSingleVertexBuffer::GetVertexDescriptor            ()
{
    return m_vertexDescriptor;
}

// *********************************
//
VertexBuffer *            RenderableArrayDataSingleVertexBuffer::GetVertexBuffer                ()
{
    return m_vertexBuffer;
}

}
