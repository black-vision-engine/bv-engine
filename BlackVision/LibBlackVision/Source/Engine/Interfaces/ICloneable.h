#pragma once

#include "CoreDEF.h"

namespace bv {

class ICloneable;
DEFINE_PTR_TYPE(ICloneable)
DEFINE_CONST_PTR_TYPE(ICloneable)

class ICloneable
{
public:
    virtual ICloneablePtr                        Clone() const                   = 0;
};


template< typename T >
std::shared_ptr< T > Clone( const T & t )
{
    return std::dynamic_pointer_cast< T >( t.Clone() );
}


template< typename T >
std::shared_ptr< T > Clone( const std::shared_ptr< T > & t )
{
    return std::shared_ptr< T >( Clone( *t ) );
}


}
