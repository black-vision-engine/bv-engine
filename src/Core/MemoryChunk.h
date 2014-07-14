#pragma once

#include <memory>

#include "System/BasicTypes.h"

namespace bv
{

class MemoryChunk
{
private:
    char *          m_memory;

public:

    const char *    Get             () const;
    char *          GetWritable     ();


    explicit        MemoryChunk     ( char* mem );
                    ~MemoryChunk    ();

};

DEFINE_PTR_TYPE( MemoryChunk )
DEFINE_CONST_PTR_TYPE( MemoryChunk )


} // bv