#pragma once

#include "DataBuffer.h"
#include <cassert>
#include <string>

namespace bv
{

class IndexBuffer : public DataBuffer
{
private:
   
    unsigned int         m_curIndex;

protected:

    unsigned int         m_offset;

public:

                IndexBuffer	( unsigned int numIndices, unsigned int indexSize, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_STATIC );
    virtual		~IndexBuffer();

    char *		    Data		() const;

    void		    SetOffset	( unsigned int offset );
    unsigned int	Offset		() const;

    void        AddIndex    ( unsigned int i)
    {
        assert(m_curIndex < m_numElts);
        memcpy(m_data + m_curIndex * m_eltSize, &i, m_eltSize);
        m_curIndex++;
    }

};

}
