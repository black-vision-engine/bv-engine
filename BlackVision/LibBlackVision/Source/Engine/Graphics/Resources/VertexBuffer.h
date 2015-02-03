#pragma once

#include "Mathematics/glm_inc.h"

#include "Engine/Graphics/Resources/DataBuffer.h"


namespace bv
{

class VertexBuffer : public DataBuffer
{
private:
    
    unsigned int     m_curVertex;

public:
            VertexBuffer	( unsigned int numVertices, unsigned int vertexSize, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_STATIC );
    virtual ~VertexBuffer	();

    char *	Data			() const;
    void    WriteToBuffer   ( const char * src, unsigned int size );

};

}
