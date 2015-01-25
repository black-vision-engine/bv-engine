#pragma once


namespace bv
{

class DataBuffer
{
public:

    // FIXME: consider raw depth buffers, data buffers and no DEPTHSTENCIL buffer
    enum class Semantic : int
    {
        S_STATIC,
        S_DYNAMIC,
        S_DEPTHSTENCIL,
        S_TEXTURE_STATIC,
        S_TEXTURE_DYNAMIC,
        S_TEXTURE_STREAMING_READ,
        S_TEXTURE_STREAMING_WRITE,
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

