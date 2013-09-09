#pragma once

#include "DataBuffer.h"
#include "glm/glm.hpp"

namespace bv
{

class VertexBuffer : public DataBuffer
{
private:
    int     m_curVertex;

public:
            VertexBuffer	( int numVertices, int vertexSize, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_STATIC );
    virtual ~VertexBuffer	();

    char *	Data			() const;

    template<class VertexType>
    void    AddVertex       (const VertexType& data)
    {
        assert(false);
    }

    template<>
    void    AddVertex<glm::vec3>(const glm::vec3& data)
    {
            assert(m_curVertex < m_numElts);
            memcpy(m_data + m_curVertex * m_eltSize, &data, m_eltSize);
            m_curVertex++;
    }

};

}
