#include "RenderableEntity.h"

#include "VertexDescriptor.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderableEffect.h"

namespace bv {

// *********************************
//
RenderableEntity::RenderableEntity          ( RenderableType type, VertexArray * vao, VertexDescriptor * vd, VertexBuffer * vb, IndexBuffer * ib, RenderableEffect * effect )
    : m_type( type )
    , m_vDesc( vd )
    , m_vBuf( vb )
    , m_iBuf( ib )
    , m_vao( vao )
    , m_effect( effect )
{
}

// *********************************
//
RenderableEntity::~RenderableEntity         ()
{
}

// *********************************
//
RenderableEntity::RenderableType RenderableEntity::GetType          () const
{
    return m_type;
}

}
