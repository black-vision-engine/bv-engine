#include "MemoryChunk.h"

namespace bv
{

MemoryChunk::MemoryChunk     ( char* mem )
    : m_memory( mem )
{
}

MemoryChunk::~MemoryChunk    ()
{
    delete [] m_memory;
}

const char *    MemoryChunk::Get            () const
{
    return m_memory;
}

char *          MemoryChunk::GetWritable    ()
{
    return m_memory;
}


} // bv