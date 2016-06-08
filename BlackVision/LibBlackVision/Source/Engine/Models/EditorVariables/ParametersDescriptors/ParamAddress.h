#pragma once

#include "Engine/Events/Events.h"

#include <string>

class ISerializer;
class IDeserializer;

namespace bv
{

struct ParameterAddress
{
    ParamKeyEvent::TargetType       ParamTargetType;
    std::string                     NodeName;
    std::string                     PluginName;
    std::string                     ParamName;
    std::string                     ParamSubName;
    std::string                     SceneName;
    UInt32                          Index;

    void                    Serialize       ( ISerializer & ser ) const;
    ParameterAddress        Create          ( const IDeserializer & deser );
};

}	// bv

