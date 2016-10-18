#pragma once

#include "Mathematics/glm_inc.h"
#include "Engine/Graphics/Resources/DataBuffer.h"
#include "CoreDEF.h"


namespace bv
{

class UniformDesc
{
public: 

    std::string     name;
    UInt32          offset;

    bool operator < ( const UniformDesc & other ) const;

};

class UniformBlockLayout
{
public:

    Int32                           size;
    std::string                     blockName;
    std::vector< UniformDesc >      uniformDescs;

};

DEFINE_UPTR_TYPE( UniformBlockLayout )

class UniformBuffer
{
private:
    
    char *		                m_data;
    DataBuffer::Semantic	    m_semantic;

    const UniformBlockLayout *  m_layout;

public:
                                UniformBuffer	( const UniformBlockLayout * blockLayout, DataBuffer::Semantic semantic );
    virtual                     ~UniformBuffer	();

    SizeType	                Size		    () const;
    DataBuffer::Semantic	    GetSemantic	    () const;

    const UniformBlockLayout *  GetLayout	    () const;
    
    const char *	            GetData			() const;
    void                        WriteData       ( const char * srcData, SizeType srcSize, UInt32 destOffset = 0 );

};

DEFINE_UPTR_TYPE( UniformBuffer )

}
