#pragma once

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include <string>


namespace bv
{
namespace model {

class BasicNode;
class BasePlugin;
DEFINE_PTR_TYPE( BasicNode )
DEFINE_PTR_TYPE( BasePlugin )

} // model    
    
namespace nodelogic
{


struct ParameterBinding
{
    model::BasicNodePtr         Node;
    model::BasePluginPtr        Plugin;

    model::IParameterPtr        Parameter;
    model::IParameterPtr        ValueSrc;

    std::string                 SourceName;
};


}   // nodelogic
}	// bv
