#pragma once

#include <string>

#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class IParameter
{
public:

    virtual const std::string &     GetName         () const = 0;
    virtual ModelParamType          GetType         () const = 0;

    virtual void *                  QueryParamTyped () = 0;

    virtual ~IParameter() {}

};

// *********************************
//
template< typename ParamType >
ParamType * QueryTypedParam( IParameter * param )
{
    if( param->GetType() != ParamType::Type() )
    {
        return nullptr;
    }

    return static_cast< ParamType * >( param->QueryParamTyped() );
}

} //model
} //bv
