#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.inl"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

namespace bv {
namespace model {

class BlendingModeHelper
{
public:

    static const std::string        PARAM_BLEND_MODE;

    enum BlendMode : int
    {
        Alpha = 0,
        Normal,
        Lighten,        ///< Blend equation max needed
        Darken,         ///< Blend equation max needed
        Multiply,
        Average,
        Add,
        Subtract,
        Difference,
        Negation,
        Screen,
        Exclusion,
        Overlay,
        SoftLight,
        HardLight,
        ColorDodge,
        ColorBurn,
        LinearDodge,
        LinearBurn,
        LinearLight,
        VividLight,
        PinLight,
        HardMix,
        Reflect,
        Glow,
        Phoenix
    };



public:
    static void         SetBlendRendererContext   ( DefaultPixelShaderChannelPtr psc, IParameterPtr param );

};


typedef ParamEnum< BlendingModeHelper::BlendMode > ParamEnumBlendMode;


DEFINE_ENUM_SET_PARAMETER( BlendingModeHelper::BlendMode );


// ***********************
//
inline VoidPtr    ParamEnumBlendMode::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
inline static IParameterPtr        ParametersFactory::CreateTypedParameter< BlendingModeHelper::BlendMode >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< BlendingModeHelper::BlendMode >( name, timeline );
}



}   // model
}	// bv
