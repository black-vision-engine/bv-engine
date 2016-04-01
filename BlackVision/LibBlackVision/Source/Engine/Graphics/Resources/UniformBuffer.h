#pragma once

#include "Mathematics/glm_inc.h"
#include "Engine/Graphics/Resources/DataBuffer.h"
#include "CoreDEF.h"


namespace bv
{

class UniformBuffer
{
private:
    
    char *		            m_data;
    SizeType                m_size;
    DataBuffer::Semantic	m_semantic;

public:
                 UniformBuffer	( SizeType size, DataBuffer::Semantic semantic );
    virtual     ~UniformBuffer	();

    SizeType	            Size		    () const;
    DataBuffer::Semantic	GetSemantic	    () const;
    
    char *	    Data			() const;
    void        WriteToBuffer   ( const char * src, SizeType size );
	
};

}
