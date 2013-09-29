#pragma once

#include "RenderableArrayData.h"

namespace bv
{

class VertexArray;

class RenderableArrayDataArrays : public RenderableArrayData
{
private:

    std::vector< VertexArray * >    m_vaoVec;

public:

    const VertexArray *     GetVAO( int ccNum ) const;

};

}
