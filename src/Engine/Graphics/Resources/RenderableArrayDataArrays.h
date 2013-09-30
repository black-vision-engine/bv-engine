#pragma once

#include "RenderableArrayData.h"

namespace bv
{

class VertexBuffer;
class VertexDescriptor;
class VertexArraySingleVertexBuffer;

class RenderableArrayDataArraysSingleVertexBuffer : public RenderableArrayDataSingleVertexBuffer
{
private:

    std::vector< VertexArraySingleVertexBuffer * >    m_vaoVec;

public:

                                            RenderableArrayDataArraysSingleVertexBuffer     ( VertexBuffer * vb, VertexDescriptor * vd );
                                            ~RenderableArrayDataArraysSingleVertexBuffer    ();

    const VertexArraySingleVertexBuffer *   VAO                                             ( int ccNum ) const;
    void                                    AddVAO                                          ( VertexArraySingleVertexBuffer * vao );

    virtual unsigned int                    GetNumConnectedComponents                       () const;
    virtual unsigned int                    GetNumVerticesInConnectedComponent              ( unsigned int ccNum ) const;

};

}
