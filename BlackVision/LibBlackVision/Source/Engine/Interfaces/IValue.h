#pragma once

#include <cassert>
#include <string>
#include <memory>

#include "Engine/Types/Enums.h"
#include "CoreDEF.h"


namespace bv {

class IValue;
typedef std::shared_ptr< const IValue > IValueConstPtr;
DEFINE_PTR_TYPE(IValue)

class IValue
{
public:

    virtual ParamType           GetType         ()  const = 0;
    virtual const char *        GetData         ()  const = 0;
    virtual const std::string & GetName         ()  const = 0;  

    virtual VoidPtr             QueryValueTyped () = 0;

    virtual ~IValue(){}

};

// *********************************
//
template< typename ValueTypePtr >
ValueTypePtr QueryTypedValue( IValuePtr val )
{
    if( val->GetType() != ValueTypePtr::element_type::Type() )
    {
        return nullptr;
    }

    return std::static_pointer_cast< ValueTypePtr::element_type >( val->QueryValueTyped() );
}

// *********************************
//
template< typename ValueTypeConstPtr >
ValueTypeConstPtr QueryTypedValue( IValueConstPtr val )
{
    if( val->GetType() != ValueTypeConstPtr::element_type::Type() )
    {
        return nullptr;
    }

    return std::static_pointer_cast< ValueTypeConstPtr::element_type >( std::const_pointer_cast< IValue >( val )->QueryValueTyped() );
}


// *********************************
//
template< typename ValueType >
ValueType * QueryTypedValue( IValue * val )
{
    if( val->GetType() != ValueType::Type() )
    {
        return nullptr;
    }

    return static_cast< ValueType * >( val->QueryValueTyped().get() );
}

// *********************************
//
template< typename ValueType >
const ValueType * QueryTypedValue( const IValue * val )
{
    if( val->GetType() != ValueType::Type() )
    {
        return nullptr;
    }

    return static_cast< const ValueType * >( const_cast< IValue * >( val )->QueryValueTyped().get() );
}

} // bv
