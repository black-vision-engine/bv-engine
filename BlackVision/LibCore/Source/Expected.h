#pragma once

#include "Exception.h"
#include <assert.h>

// ************************
//
template< typename HamType, typename ErrorType = bv::ExceptionPtr >
struct Expected
{
    union {
        HamType ham;
        ErrorType spam;
    };

    bool isValid;

    Expected( HamType h ) : ham( h ), isValid( true ) {}
    Expected() : isValid( false ), spam( nullptr ) {}
    Expected(const Expected<HamType, ErrorType>& that) : isValid( that.isValid ) { if( isValid ) new( &ham ) HamType( that.ham ); else new( &spam ) ErrorType( that.spam ); }
    ~Expected() { if (isValid) ham.~HamType(); else spam.~ErrorType(); }
    bool operator==( const HamType& that ) { return isValid && ham==that; }
    bool operator!=( const HamType& that ) { return !(*this==that); }
    operator HamType&() { if( !isValid ) assert( false ); return ham; }
};

