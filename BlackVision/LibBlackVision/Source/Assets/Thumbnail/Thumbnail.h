#pragma once

#include "CoreDEF.h"
#include "Memory/MemoryChunk.h"

namespace bv
{

class ISerializer;

class Thumbnail
{
public:
    explicit    Thumbnail   () {}
    virtual     ~Thumbnail  () {}

    virtual MemoryChunkConstPtr     Data        () const = 0;
    virtual const std::string &     DataBase64  () const = 0;

    virtual void            Serialize   ( ISerializer & ) const
    { 
        assert( false ); // FIXME: Remove this body when will be overrided in every sub-classes.
        // Override in sub-classes;
    };
};

DEFINE_CONST_PTR_TYPE( Thumbnail )

} // bv