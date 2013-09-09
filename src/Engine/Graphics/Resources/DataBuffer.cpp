#include "DataBuffer.h"
#include <cassert>

namespace bv {

// *********************************
//
DataBuffer::DataBuffer ()
	: m_numElts( 0 )
	, m_eltSize( 0 )
	, m_semantic( DataBuffer::Semantic::S_TOTAL )
	, m_size( 0 )
	, m_data( nullptr )	
{
}

// *********************************
//
DataBuffer::DataBuffer	( int numElts, int eltSize, DataBuffer::Semantic semantic )
	: m_numElts( numElts )
	, m_eltSize( eltSize )
	, m_semantic( semantic )
	, m_size( numElts * eltSize )
	, m_data( nullptr )	
{
	assert( numElts > 0 );
	assert( eltSize > 0 );

	m_data = new char[ m_size ];
}

// *********************************
//
DataBuffer::~DataBuffer ()
{
    delete[] m_data;
}

// *********************************
//
int DataBuffer::NumElements					() const
{
	return m_numElts;
}

// *********************************
//
int DataBuffer::ElementSize					() const
{
    return m_eltSize;
}

// *********************************
//
DataBuffer::Semantic DataBuffer::GetSemantic() const
{
	return m_semantic;
}

// *********************************
//
int DataBuffer::Size						() const
{
    return m_size;
}

// *********************************
//
const char * DataBuffer::Data				() const
{
    return m_data;
}

}