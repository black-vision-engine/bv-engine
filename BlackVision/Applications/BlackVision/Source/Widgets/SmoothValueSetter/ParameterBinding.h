#pragma once

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Types/Enums.h"

#include <string>


namespace bv
{
    
namespace nodelogic
{


struct ParameterBinding
{
    std::string                     Node;
    std::string                     Plugin;

    model::IParameterPtr            Parameter;
    IValuePtr                       ValueSrc;

    ParameterBinding()
    {
        Parameter = nullptr;
        ValueSrc = nullptr;
    }
};


}   // nodelogic
}	// bv
