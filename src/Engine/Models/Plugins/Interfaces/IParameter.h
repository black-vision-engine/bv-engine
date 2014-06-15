#pragma once

#include <string>
#include <memory>

#include "Engine/Types/Enums.h"

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class IParameter;
typedef std::shared_ptr< IParameter > IParameterPtr;

class IParameter
{
public:

    virtual const std::string &     GetName         () const = 0;
    virtual ModelParamType          GetType         () const = 0;

    virtual ITimeEvaluatorPtr       GetTimeEvaluator() = 0;
    virtual void                    SetTimeEvaluator( ITimeEvaluatorPtr timeEvaluator ) = 0;

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

// *********************************
//
template< typename ParamType >
const ParamType * QueryTypedParam( const IParameter * param )
{
    if( param->GetType() != ParamType::Type() )
    {
        return nullptr;
    }

    return static_cast< ParamType * >( const_cast< IParameter * >( param )->QueryParamTyped() );
}

void    SetParamTimeline( IParameter * param, ITimeEvaluatorPtr timeline );

} //model
} //bv
