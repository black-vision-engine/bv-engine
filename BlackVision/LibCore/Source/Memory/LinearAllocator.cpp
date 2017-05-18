#include "stdafxCore.h"
#include "LinearAllocator.h"



namespace bv
{


// ***********************
//
LinearAllocator::LinearAllocator( SizeType initSize )
    : m_offset( 0 )
{
    m_memChunks.push_back( MemoryChunk( nullptr, 0 ) );
    
    if( initSize != 0 )
    {
        m_memChunks[ 0 ].Allocate( initSize );
    }
}

// ***********************
//
LinearAllocator::~LinearAllocator()
{}

// ***********************
//
void        LinearAllocator::FreeAll()
{
    //Allocate< MemoryChunk >();
    //Free( &m_memChunks[ 0 ] );
    assert( false );

    m_offset = 0;
    //for(  )
}

// ***********************
//
void        LinearAllocator::AllocNewChunk()
{
    m_offset = 0;
    auto chunkSize = m_memChunks.size();

    // Note: we have to move all MemoryChunk. Copy contructor dealocates memory.
    std::vector< MemoryChunk > newVec;
    newVec.reserve( chunkSize + 1 );

    for( SizeType i = 0; i < chunkSize; ++i )
    {
        newVec.push_back( std::move( m_memChunks[ i ] ) );
    }

    newVec.push_back( MemoryChunk( nullptr, 0 ) );
    newVec[ chunkSize ].Allocate( newVec[ 0 ].Size() );

    m_memChunks = std::move( newVec );
}

}   // bv
