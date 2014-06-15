#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IParameter.h"

namespace bv { namespace model {

class IParamSet
{
public:

    virtual std::vector< IParameterPtr > &      GetParameters       () = 0;
    virtual IParameterPtr                       GetParameter        ( const std::string & name ) = 0;

    virtual ~IParamSet(){}

};

} //model
} //bv
