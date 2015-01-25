#pragma once

#include "Engine/Graphics/Resources/RenderableArrayData.h"

namespace bv
{

class VertexBuffer;
class VertexDescriptor;
class IndexBuffer;

//FIXME: add public setter interface
class RenderableArrayDataElements : public RenderableArrayDataSingleVertexBuffer
{
private:
    
    //FIXME: delete or uncomment if useful
    //std::vector< unsigned int >     m_numVerticesInCC;
    //std::vector< unsigned int >     m_ccOffsets;

    std::vector< IndexBuffer * >    m_inedxBuffersVec;

public:

                                        RenderableArrayDataElements         ( VertexBuffer * vb, VertexDescriptor * vd );

    const IndexBuffer *                 GetIndexBuffer                      ( int ccNum ) const;

    //FIXME: delete or uncomment if useful
    //unsigned int                        GetCCOffset                         ( int ccNum ) const
    //unsigned int                        GetCCNumVertices                    ( int ccNum ) const

    virtual unsigned int                GetNumConnectedComponents           () const;
    virtual unsigned int                GetNumVerticesInConnectedComponent  ( unsigned int ccNum ) const;

};

}
