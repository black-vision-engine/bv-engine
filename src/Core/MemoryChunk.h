#pragma once

#include <memory>

#include "System/BasicTypes.h"

namespace bv
{

class MemoryChunk;

DEFINE_PTR_TYPE( MemoryChunk )
DEFINE_CONST_PTR_TYPE( MemoryChunk )

class MemoryChunk
{
private:

    char *          m_memory;
    unsigned int    m_size;

public:

    explicit                MemoryChunk     ( char * mem, unsigned int size );
                            ~MemoryChunk    ();

    void                    SetDataChunk    ( char * data, unsigned int size );
    void                    Allocate        ( unsigned int size );

    unsigned int            Size            () const;

    const char *            Get             () const;
    char *                  GetWritable     ();

    static  MemoryChunkPtr  Create          ( unsigned int size );

private:

    void            DeleteMemory    ();

};

} // bv