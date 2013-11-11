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

	unsigned int	m_numElts;
    unsigned int	m_eltSize;

    Semantic	    m_semantic;

	unsigned int    m_size;
    
	char *		    m_data;

protected:

    DataBuffer					    ();
    DataBuffer					    ( unsigned int numelts, unsigned int eltSize, Semantic semantic );

public:

    virtual			~DataBuffer     ();

    unsigned int	NumElements     () const;
    unsigned int	ElementSize     () const;
    Semantic		GetSemantic	    () const;

    unsigned int	Size		    () const;
    const char *	Data		    () const;

    void            Reinitialize    ( unsigned int numElts, unsigned int eltSize, Semantic semantic );

};

}

