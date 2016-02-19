#pragma once

#include <assert.h>

// ************************
//
template< typename T >
struct Expected
{
    T ham;
    bool isValid;

    Expected( T h ) : ham( h ), isValid( true ) {}
    Expected() : isValid( false ) {}
    bool operator==( const T& that ) { return isValid && ham==that; }
    bool operator!=( const T& that ) { return !(*this==that); }
    operator T&() { if( !isValid ) assert( false ); return ham; }
};

