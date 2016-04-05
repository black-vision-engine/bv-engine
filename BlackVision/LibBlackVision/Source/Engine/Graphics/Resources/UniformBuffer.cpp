#include "stdafx.h"

#include "UniformBuffer.h"


namespace bv {


Int32      UniformBuffer::TypeAlignMap[ ( UInt32 )ParamType::PT_TOTAL ] =
{
    4,  //PT_FLOAT1,
    8,  //PT_FLOAT2,
    16, //PT_FLOAT3,
    16, //PT_FLOAT4,
    16, //PT_MAT2,
    16, //PT_MAT3,
    16, //PT_MAT4,
    4,  //PT_INT,
    4,  //PT_BOOL,
    4,  //PT_ENUM,
    -1, //PT_STRING,
    -1 //PT_WSTRING,
};

// *********************************
//
bool UniformDesc::operator < ( const UniformDesc & other ) const
{
    return offset < other.offset;
}

// *********************************
//
UniformBuffer::UniformBuffer ( const UniformBlockLayout * blockLayout, DataBuffer::Semantic semantic )
    : m_semantic( semantic )
    , m_layout( blockLayout )
{
    assert( m_layout->size > 0 );
	m_data = new char[ m_layout->size ];
}

// *********************************
//
UniformBuffer::~UniformBuffer ()
{
    delete[] m_data;
}

// *********************************
//
DataBuffer::Semantic	    UniformBuffer::GetSemantic	() const
{
    return m_semantic;
}

// *********************************
//
const UniformBlockLayout *	UniformBuffer::GetLayout	() const
{
    return m_layout;
}

// *********************************
//
SizeType                    UniformBuffer::Size	        () const
{
    return m_layout->size;
}

// *********************************
//
const char *                UniformBuffer::GetData	    () const
{
    return m_data;
}

// *********************************
//
void                        UniformBuffer::WriteData    ( const char * srcData, SizeType srcSize, UInt32 destOffset )
{
    memcpy( &m_data[ destOffset ], srcData, srcSize );
}

}
