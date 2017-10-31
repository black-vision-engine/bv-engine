#pragma once

#include "Exception.h"



namespace bv
{



// ***********************
//
class SmallException : public RuntimeException
{
    static ExceptionType        type;
public:
    static ExceptionType        Type() { return type; }

protected:
public:
    SmallException      ()
        : RuntimeException( "SmallException" )
    {}

    virtual ExceptionType       GetType         () const override                           { return Type(); }
    virtual bool                IsDerivedFrom   ( ExceptionType fromType ) const override   { return IsDerivedImpl< SmallException, RuntimeException >( fromType ); }
};

DEFINE_PTR_TYPE( SmallException );


// ***********************
//
class BigException : public RuntimeException
{
    static ExceptionType        type;
public:
    static ExceptionType        Type() { return type; }

protected:
public:
    BigException        ()
        : RuntimeException( "BigException" )
    {}

    virtual ExceptionType       GetType         () const override                           { return Type(); }
    virtual bool                IsDerivedFrom   ( ExceptionType fromType ) const override   { return IsDerivedImpl< SmallException, RuntimeException >( fromType ); }
};

DEFINE_PTR_TYPE( BigException );



// ***********************
//
class ChildishException : public BigException
{
    static ExceptionType        type;
public:
    static ExceptionType        Type() { return type; }

protected:
public:
    ChildishException        ()
        : BigException()
    {}

    virtual ExceptionType       GetType         () const override                           { return Type(); }
    virtual bool                IsDerivedFrom   ( ExceptionType fromType ) const override   { return IsDerivedImpl< ChildishException, RuntimeException >( fromType ); }
};

DEFINE_PTR_TYPE( ChildishException );




}	// bv


