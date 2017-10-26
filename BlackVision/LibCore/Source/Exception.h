#pragma once

#include <exception>
#include "CoreDEF.h"



namespace bv
{

typedef     Int32       ExceptionType;


// ***********************
//
class Exception : public std::exception
{
private:

    static ExceptionType        type;

protected:

    static ExceptionType        sInvalidType;

public:
                                Exception();

    virtual std::string         GetReason       () const = 0;
    virtual ExceptionType       GetType         () const = 0;
    virtual bool                IsDerivedFrom   ( ExceptionType fromType ) const = 0 { return fromType == type; }

    static ExceptionType        RegisterType    ();
    static bool                 IsValidType     ( ExceptionType type );

    static ExceptionType        Type            ();

protected:

    template< typename ThisType, typename ParentType >
    inline bool                         IsDerivedImpl   ( ExceptionType exType ) const;
};

DEFINE_PTR_TYPE( Exception );



typedef std::vector< ExceptionPtr > Exceptions;

// ***********************
//
class RuntimeException : public Exception
{
    static ExceptionType        type;
public:
    static ExceptionType        Type();

protected:

    std::string                 reason;

public:
                                RuntimeException    ( const std::string& reason );

    virtual std::string         GetReason       () const override;
    virtual ExceptionType       GetType         () const override;
    virtual bool                IsDerivedFrom   ( ExceptionType fromType ) const override { return IsDerivedImpl< RuntimeException, Exception >( fromType ); }
};

DEFINE_PTR_TYPE( RuntimeException );




template< typename CastType >
using Ptr = std::shared_ptr< CastType >;

template< typename CastType, typename ExceptType >
static inline Ptr< CastType >           Cast            ( const Ptr< ExceptType > & ptr );



// ========================================================================= //
// Implementation
// ========================================================================= //

// ***********************
//
template< typename CastType, typename ExceptType >
inline Ptr< CastType >                  Cast            ( const Ptr< ExceptType > & ptr )
{
    if( ptr->IsDerivedFrom( CastType::Type() ) )
        return std::static_pointer_cast< CastType >( ptr );

    return false;
}

// ***********************
//
template< typename ThisType, typename ParentType >
inline bool                             Exception::IsDerivedImpl    ( ExceptionType fromType ) const
{
    if( fromType == ThisType::Type() )
        return true;

    auto parentPtr = static_cast< const ParentType * >( this );

    // Call ParentType implementation of IsDerivedFrom function.
    return parentPtr->ParentType::IsDerivedFrom( fromType );
}


}
