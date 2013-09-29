#include "RenderableArrayDataArrays.h"

#include <cassert>
#include "Engine\Graphics\Resources\VertexArray.h"

namespace bv
{

// *********************************
//
const VertexArray *     RenderableArrayDataArrays::GetVAO( int ccNum ) const
{
    assert( ccNum >= 0 );
    assert( ccNum < (int) m_vaoVec.size() );

    return m_vaoVec[ ccNum ];
}

}
