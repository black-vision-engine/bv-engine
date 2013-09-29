#include "RenderableArrayDataElements.h"

#include <cassert>

namespace bv
{

// *********************************
//
RenderableArrayDataElements::RenderableArrayDataElements ()
    : RenderableArrayData( RenderableArrayDataEnumKind::RADEK_ELEMENTS )
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

}
