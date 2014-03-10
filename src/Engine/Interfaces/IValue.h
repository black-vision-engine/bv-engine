#pragma once

#include <cassert>
#include <string>

#include "Engine/Types/Enums.h"


namespace bv {

class IValue
{
public:

    virtual ParamType           GetType         ()  const = 0;
    virtual const char *        GetData         ()  const = 0;
    virtual const std::string & GetName         ()  const = 0;  

    virtual void *              QueryValueTyped () = 0;

    virtual ~IValue(){}

};

// *********************************
//
template< typename ValueType >
ValueType * QueryTypedValue( IValue * val )
{
    if( val->GetType() != ValueType::Type() )
    {
        return nullptr;
    }

    return static_cast< ValueType * >( val->QueryValueTyped() );
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

    return static_cast< const ValueType * >( const_cast< IValue * >( val )->QueryValueTyped() );
}

} // bv
