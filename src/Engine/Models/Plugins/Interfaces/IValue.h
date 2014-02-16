#pragma once

#include <cassert>
#include <string>

#include "Engine/Types/Enums.h"


namespace bv { namespace model {

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
    assert( val->GetType() == ValueType::Type() );

    return static_cast< ValueType * >( val->QueryValueTyped() );
}

} // model
} // bv
