#pragma once

#include "DataBuffer.h"
#include <cassert>
#include <string>

namespace bv
{

class IndexBuffer : public DataBuffer
{
private:
    int         m_curIndex;
protected:

    int         m_offset;

public:

                IndexBuffer	( int numIndices, int indexSize, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_STATIC );
    virtual		~IndexBuffer();

    char *		Data		() const;

    void		SetOffset	( int offset );
    int			Offset		() const;

    void        AddIndex    (int i)
    {
        assert(m_curIndex < m_numElts);
        memcpy(m_data + m_curIndex * m_eltSize, &i, m_eltSize);
        m_curIndex++;
    }

};

}
