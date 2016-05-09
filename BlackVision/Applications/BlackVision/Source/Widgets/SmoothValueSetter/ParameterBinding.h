#pragma once

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Types/Enums.h"

#include <string>


namespace bv
{
namespace model {

class BasicNode;
DEFINE_PTR_TYPE( BasicNode )

} // model    
    
namespace nodelogic
{


struct ParameterBinding
{
    model::BasicNodePtr             Node;
    model::IPluginPtr               Plugin;

    model::IParameterPtr            Parameter;
    IValuePtr                       ValueSrc;

    std::string                     SourceName;

    ParameterBinding()
    {
        Node = nullptr;
        Plugin = nullptr;
        Parameter = nullptr;
        ValueSrc = nullptr;
    }
};


}   // nodelogic
}	// bv
