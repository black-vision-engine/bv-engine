#include "stdafxCore.h"

#include "MemoryChunk.h"

//#define PRINT_ALLOCATIONS

namespace bv
{

// ****************************
//
MemoryChunk::MemoryChunk     ( char * mem, SizeType size )
    : m_memory( nullptr )
    , m_size( 0 )
{
    SetDataChunk( mem, size );
}

// ****************************
//
MemoryChunk::~MemoryChunk    ()
{
    DeleteMemory();
}

// ****************************
//
void            MemoryChunk::SetDataChunk   ( char * data, SizeType size )
{
    DeleteMemory();

#ifdef PRINT_ALLOCATIONS
    if( size > 0 )
        printf( "Setting %d bytes at %08X\n", size, (size_t) data );
#endif

    m_memory = data;
    m_size = size;
}

// ****************************
//
void            MemoryChunk::Allocate       ( SizeType size )
{
    DeleteMemory();

    char * memory = new char[ size ];

    SetDataChunk( memory, size );

#ifdef PRINT_ALLOCATIONS
    printf( "Allocating %d bytes at %08X\n", size, (size_t) memory );
#endif
}

// ****************************
//
SizeType		MemoryChunk::Size           () const
{
    return m_size;
}

// ****************************
//
const char *    MemoryChunk::Get            () const
{
    return m_memory;
}

// ****************************
//
char *          MemoryChunk::GetWritable    ()
{
    return m_memory;
}

// ****************************
//
void            MemoryChunk::Clear          ()
{
    memset( m_memory, 0, m_size );
}

// ****************************
//
MemoryChunkPtr  MemoryChunk::Create         ( SizeType size )
{
    auto mc = std::make_shared< MemoryChunk >( nullptr, 0 );
    mc->Allocate( size );
    mc->Clear();
    return mc;
}

// ****************************
//
MemoryChunkPtr MemoryChunk::Create          ( char * mem, SizeType size )
{
	return std::make_shared< MemoryChunk >( mem, size );
}

// ****************************
//
MemoryChunkConstPtr MemoryChunk::EMPTY      ()
{
    static MemoryChunkPtr emptyInst = std::make_shared< MemoryChunk >( nullptr, 0 );

    return emptyInst;
}

// ****************************
//
void            MemoryChunk::DeleteMemory   ()
{
#ifdef PRINT_ALLOCATIONS
    if( m_size > 0 )
        printf( "Deleting %d bytes at %08X\n", m_size, (size_t) m_memory );
#endif

    delete [] m_memory;
    m_memory = nullptr;
    m_size = 0;
}

} // bv
