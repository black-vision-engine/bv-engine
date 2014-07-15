#include "MemoryChunk.h"

namespace bv
{

// ****************************
//
MemoryChunk::MemoryChunk     ( char * mem, unsigned int size )
    : m_memory( nullptr )
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
void            MemoryChunk::DeleteMemory   ()
{
    delete m_memory;
    m_memory = nullptr;
}

} // bv
