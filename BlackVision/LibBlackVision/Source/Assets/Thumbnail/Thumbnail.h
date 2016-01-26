#pragma once

#include "CoreDEF.h"

namespace bv
{

class Thumbnail;
DEFINE_CONST_PTR_TYPE( Thumbnail )

class ISerializer;

class Thumbnail
{
public:
    explicit    Thumbnail   () {}
    virtual     ~Thumbnail  () {}

    virtual const char *    Data        () const = 0;
    virtual const char *    DataBase64  () const = 0;

    virtual void            Serialize   ( ISerializer & ) const
    { 
        assert( false ); // FIXME: Remove this body when will be overrided in every sub-classes.
        // Override in sub-classes;
    };
};

} // bv