#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.inl"
#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.h"
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
        Lighten,
        Darken,
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


template<>
inline bool SetParameter< BlendingModeHelper::BlendMode >( IParameterPtr param, TimeType t, const BlendingModeHelper::BlendMode & val )
{
    ParamEnumBlendMode * typedParam = QueryTypedParam< std::shared_ptr< ParamEnumBlendMode > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}

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

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"


}   // model
}	// bv
