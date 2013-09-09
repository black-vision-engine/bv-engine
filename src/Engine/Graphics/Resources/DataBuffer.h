#pragma once

namespace bv
{

class DataBuffer
{
public:

    enum class Semantic : int
    {
        S_STATIC,
        S_DYNAMIC,
        S_RENDERTARGET,
        S_DEPTHSTENCIL,
        S_TEXTURE,
        S_TOTAL
    };

protected:

	int			m_numElts;
    int			m_eltSize;

    Semantic	m_semantic;

	int			m_size;
    
	char *		m_data;

protected:

    DataBuffer					();
    DataBuffer					( int numelts, int eltSize, Semantic semantic );

public:

    virtual			~DataBuffer ();

    int				NumElements () const;
    int				ElementSize () const;
    Semantic		GetSemantic	() const;

    int				Size		() const;
    const char *	Data		() const;
};

}

