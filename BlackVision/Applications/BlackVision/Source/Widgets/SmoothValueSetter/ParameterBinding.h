#pragma once

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/Plugin.h"
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
    model::BasicNodePtr         Node;
    model::IPluginPtr           Plugin;

    model::IParameterPtr        Parameter;
    model::IParameterPtr        ValueSrc;

    std::string                 SourceName;
};


}   // nodelogic
}	// bv
