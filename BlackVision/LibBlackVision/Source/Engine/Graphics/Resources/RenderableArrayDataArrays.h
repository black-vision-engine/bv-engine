#pragma once

#include "Engine/Graphics/Resources/RenderableArrayData.h"

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

                                            RenderableArrayDataArraysSingleVertexBuffer     ( VertexArraySingleVertexBuffer * vao );
                                            ~RenderableArrayDataArraysSingleVertexBuffer    ();

    const VertexArraySingleVertexBuffer *   VAO                                             () const;
    VertexArraySingleVertexBuffer *         VAO                                             ();
    void                                    SetVAO                                          ( VertexArraySingleVertexBuffer * vao );

    virtual SizeType                        GetNumConnectedComponents                       () const;
    virtual unsigned int                    GetNumVerticesInConnectedComponent              ( unsigned int ccNum ) const;

};

}
