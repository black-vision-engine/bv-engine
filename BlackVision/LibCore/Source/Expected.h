#pragma once

#include "Exception.h"
#include <assert.h>

namespace bv
{

namespace impl
{

// ************************
//
template< typename HamType, typename ErrorType = bv::ExceptionPtr >
class ExpectedBase
{
protected:
    union
    {
        HamType ham;
        ErrorType spam;
    };

    bool isValid;

public:

    explicit                ExpectedBase        ();
                            ExpectedBase        ( HamType h );
                            ExpectedBase        ( ErrorType error );
                            ExpectedBase        ( const ExpectedBase< HamType, ErrorType > & that );
                            ~ExpectedBase       ();

    bool                    IsValid             ();
    std::string             GetErrorReason      ();

    bool                    operator==          ( const HamType & that );
    bool                    operator!=          ( const HamType & that );

    const HamType &         GetVal              ();
    operator const HamType &                    ();


public:

    static ExpectedBase     fromError           ( ErrorType error );
    static ExpectedBase     fromError           ( const std::string& reason );
    static ExpectedBase     fromError           ();
};

}

// ************************
//
template< typename HamType, typename ErrorType = bv::ExceptionPtr >
class Expected : public impl::ExpectedBase< HamType, ErrorType >
{
public:
    explicit                Expected            () 
    {}
                            Expected            ( HamType h ) 
                                : ExpectedBase( h ) 
    {}
                            Expected        ( const std::string & reason )
                                : ExpectedBase( ErrorType( new RuntimeException( reason ) ) ) 
    {}
                            Expected ( const ExpectedBase & that ) // conversion to let ExpectedBase::fromError work
                                : ExpectedBase( that ) {}
};

// ========================================================================= //
// std::string specialization
// ========================================================================= //

template<>
class Expected< std::string, bv::ExceptionPtr > : public impl::ExpectedBase< std::string, bv::ExceptionPtr >
{
public:
    explicit                Expected            () 
    {}

                            Expected            ( const std::string & value )
                                : ExpectedBase< std::string, bv::ExceptionPtr >( value ) 
    {}

                            Expected ( const Expected & that ) 
                                : ExpectedBase< std::string, bv::ExceptionPtr >( that ) 
    {}

                            Expected ( const ExpectedBase & that ) // conversion to let ExpectedBase::fromError work
                                : ExpectedBase( that )
    {}
};

// ========================================================================= //
// Implementation
// ========================================================================= //

namespace impl
{

template< typename HamType, typename ErrorType >
ExpectedBase< HamType, ErrorType >::ExpectedBase        () 
    : isValid( false )
    , spam( nullptr ) 
{}

template< typename HamType, typename ErrorType >
ExpectedBase< HamType, ErrorType >::ExpectedBase        ( HamType h ) 
    : isValid( true ), ham( h ) 
{}

template< typename HamType, typename ErrorType >
ExpectedBase< HamType, ErrorType >::ExpectedBase        ( ErrorType error ) 
    : isValid( false ), spam( error ) 
{}

template< typename HamType, typename ErrorType >
ExpectedBase< HamType, ErrorType >::ExpectedBase        ( const ExpectedBase< HamType, ErrorType > & that ) 
    : isValid( that.isValid ) 
{ 
    if( isValid ) 
        new( &ham ) HamType( that.ham ); 
    else 
        new( &spam ) ErrorType( that.spam ); 
}

template< typename HamType, typename ErrorType >
ExpectedBase< HamType, ErrorType >::~ExpectedBase       () 
{ 
    if( isValid ) 
        ham.~HamType(); 
    else 
        spam.~ErrorType(); 
}

template< typename HamType, typename ErrorType >
bool ExpectedBase< HamType, ErrorType >::IsValid() 
{
    return isValid;
}

template< typename HamType, typename ErrorType >
std::string             ExpectedBase< HamType, ErrorType >::GetErrorReason  () 
{ 
    return spam->GetReason(); 
}

template< typename HamType, typename ErrorType >
bool        ExpectedBase< HamType, ErrorType >::operator==      ( const HamType & that ) 
{ 
    return isValid && ham == that; 
}

template< typename HamType, typename ErrorType >
bool        ExpectedBase< HamType, ErrorType >::operator!=      ( const HamType & that ) 
{ 
    return !( *this == that ); 
}

template< typename HamType, typename ErrorType >
const HamType &         ExpectedBase< HamType, ErrorType >::GetVal          () 
{ 
    if( !isValid ) 
        assert( false );  // FIXME: error handling(?)
    return ham; 
}

template< typename HamType, typename ErrorType >
ExpectedBase< HamType, ErrorType >::operator const HamType &  ( ) 
{ 
    return GetVal(); 
}

// ========================================================================= //
// static ExpectedBase methods
// ========================================================================= //

template< typename HamType, typename ErrorType >
ExpectedBase< HamType, ErrorType >     ExpectedBase< HamType, ErrorType >::fromError       ( ErrorType error ) 
{ 
    ExpectedBase< HamType, ErrorType > ret; 
    ret.spam = error; 
    return ret; 
}

template< typename HamType, typename ErrorType >
ExpectedBase< HamType, ErrorType >     ExpectedBase< HamType, ErrorType >::fromError       ( const std::string& reason ) 
{ 
    return fromError( bv::ExceptionPtr( new RuntimeException ( reason ) ) ); 
}

template< typename HamType, typename ErrorType >
ExpectedBase< HamType, ErrorType >     ExpectedBase< HamType, ErrorType >::fromError       () 
{ 
    return ExpectedBase< HamType, ErrorType >(); 
}

}

}	// bv