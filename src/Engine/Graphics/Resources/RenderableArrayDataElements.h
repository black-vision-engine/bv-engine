#pragma once

#include "RenderableArrayData.h"

namespace bv
{

class IndexBuffer;

class RenderableArrayDataElements : public RenderableArrayData
{
private:

    std::vector< IndexBuffer * >    m_inedxBuffersVec;

public:

                            RenderableArrayDataElements ();

    const IndexBuffer *     GetIndexBuffer              ( int ccNum ) const;

};

}
