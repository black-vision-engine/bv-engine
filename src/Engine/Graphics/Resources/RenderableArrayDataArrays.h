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

                            RenderableArrayDataArrays   ();

    const VertexArray *     GetVAO                      ( int ccNum ) const;

};

}
