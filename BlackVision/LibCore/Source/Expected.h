#pragma once

#include "Exception.h"
#include <assert.h>

// ************************
//
template< typename T, typename E = bv::ExceptionPtr >
struct Expected
{
    union {
        T ham;
        E spam;
    };

    bool isValid;

    Expected( T h ) : ham( h ), isValid( true ) {}
    Expected() : isValid( false ), spam( nullptr ) {}
    Expected(const Expected<T, E>& that) : isValid( that.isValid ) { if( isValid ) ham = that.ham; else spam = that.spam; }
    ~Expected() { if (isValid) ham.~T(); else spam.~E(); }
    bool operator==( const T& that ) { return isValid && ham==that; }
    bool operator!=( const T& that ) { return !(*this==that); }
    operator T&() { if( !isValid ) assert( false ); return ham; }
};

