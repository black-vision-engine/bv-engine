#pragma once

namespace bv {

class ICloneable
{
public:
    virtual ICloneable*                         Clone() const                   = 0;
};


template< typename T >
T* Clone( const T& t )
{
    return dynamic_cast< T* >( t.Clone() );
}


template< typename T >
std::shared_ptr< T > Clone( std::shared_ptr< T > t )
{
    return std::shared_ptr< T >( Clone( *t ) );
}


}
