#pragma once

#include "Serialization/SerializationHelper.h"
#include "Engine/Events/CommonEventsStrings.h"

#include <string>

class ISerializer;
class IDeserializer;


namespace bv
{

struct ParameterAddress
{
public:
    typedef enum
    {
        PluginParam,
        GlobalEffectParam,
        ResourceParam,
        LightParam,
        NodeLogicParam,
        CameraParam,
        FailTarget      ///< Wrong target type
    } TargetType;

public:
    ParameterAddress::TargetType    ParamTargetType;
    std::string                     NodeName;
    std::string                     PluginName;
    std::string                     ParamName;
    std::string                     ParamSubName;
    std::string                     SceneName;
    UInt32                          Index;

    void                        Serialize       ( ISerializer & ser ) const;
    static ParameterAddress     Create          ( const IDeserializer & deser );

    friend bool                 operator<       ( const ParameterAddress& address1, const ParameterAddress& address2 );
};

DECLARE_ENUM_SERIALIZATION( ParameterAddress::TargetType );

}	// bv

