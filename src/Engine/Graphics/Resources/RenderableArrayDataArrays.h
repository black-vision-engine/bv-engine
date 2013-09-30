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

    VertexArraySingleVertexBuffer *         m_vao;

public:

                                            RenderableArrayDataArraysSingleVertexBuffer     ( VertexBuffer * vb, VertexDescriptor * vd );
                                            ~RenderableArrayDataArraysSingleVertexBuffer    ();

    const VertexArraySingleVertexBuffer *   VAO                                             () const;
    void                                    SetVAO                                          ( VertexArraySingleVertexBuffer * vao );

    virtual unsigned int                    GetNumConnectedComponents                       () const;
    virtual unsigned int                    GetNumVerticesInConnectedComponent              ( unsigned int ccNum ) const;

};

}
