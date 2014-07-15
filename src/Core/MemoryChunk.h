#pragma once

#include <memory>

#include "System/BasicTypes.h"

namespace bv
{

class MemoryChunk
{
private:

    char *          m_memory;
    unsigned int    m_size;

public:

    explicit        MemoryChunk     ( char * mem, unsigned int size );
                    ~MemoryChunk    ();

    void            SetDataChunk    ( char * data, unsigned int size );
    void            Allocate        ( unsigned int size );

    unsigned int    Size            () const;

    const char *    Get             () const;
    char *          GetWritable     ();

private:

    void            DeleteMemory    ();

};

DEFINE_PTR_TYPE( MemoryChunk )
DEFINE_CONST_PTR_TYPE( MemoryChunk )


} // bv