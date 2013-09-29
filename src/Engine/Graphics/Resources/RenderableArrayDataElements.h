#pragma once

#include "RenderableArrayData.h"

namespace bv
{

class IndexBuffer;

class RenderableArrayDataElements : public RenderableArrayData
{
private:

    std::vector< IndexBuffer * >    m_inexBuffersVec;

public:

    const IndexBuffer *     GetIndexBuffer( int ccNum ) const;

};

}
