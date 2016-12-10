#pragma once

#include <memory>

#include "CoreDEF.h"

namespace bv
{

class MemoryChunk;

DEFINE_PTR_TYPE( MemoryChunk )
DEFINE_CONST_PTR_TYPE( MemoryChunk )

typedef std::vector< MemoryChunkConstPtr > MemoryChunkVector;

class MemoryChunk
{
private:

    char *          m_memory;
    SizeType		m_size;

public:

    explicit                    MemoryChunk     ( char * mem, SizeType size );
                                ~MemoryChunk    ();

    void                        SetDataChunk    ( char * data, SizeType size );
    void                        Allocate        ( SizeType size );

    SizeType					Size            () const;

    const char *                Get             () const;
    char *                      GetWritable     ();

    void                        Clear           ();

    static  MemoryChunkPtr      Create          ( SizeType size );
	static  MemoryChunkPtr      Create          ( char * mem, SizeType size );

    static  MemoryChunkConstPtr EMPTY           ();

private:

    void            DeleteMemory    ();

};

} // bv