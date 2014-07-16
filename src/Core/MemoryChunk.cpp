#include "MemoryChunk.h"

//#define PRINT_ALLOCATIONS

namespace bv
{

// ****************************
//
MemoryChunk::MemoryChunk     ( char * mem, unsigned int size )
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
void            MemoryChunk::SetDataChunk   ( char * data, unsigned int size )
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
void            MemoryChunk::Allocate       ( unsigned int size )
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
unsigned int    MemoryChunk::Size           () const
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
MemoryChunkPtr  MemoryChunk::Create         ( unsigned int size )
{
    auto mc = std::make_shared< MemoryChunk >( nullptr, 0 );
    mc->Allocate( size );

    return mc;
}

// ****************************
//
MemoryChunkConstPtr MemoryChunk::EMPTY      ()
{
    static MemoryChunkPtr emptyInst = std::make_shared< MemoryChunk >( nullptr, 0 );
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
