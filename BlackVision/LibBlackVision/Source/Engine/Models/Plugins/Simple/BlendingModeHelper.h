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

    enum BlendMode
    {
        BM_Normal = 0,
        BM_Lighten,
        BM_Darken,			// Blending function GL_MIN needed.
        BM_Multiply,		// Blending function GL_MAX needed.
        BM_Average,
        BM_Add,
        BM_Substract,		// Blending function GL_SUBTRACT needed.
        BM_Difference,
        BM_Negation,
        BM_Exclusion,
        BM_Screen,
        BM_Overlay,
        BM_SoftLight,
        BM_HardLight,
        BM_ColorDodge,
        BM_ColorBurn,
        BM_LinearDodge,
        BM_LinearBurn,
        BM_LinearLight,
        BM_VividLight,
        BM_PinLight,
        BM_HardMix,
        BM_Reflect,
        BM_Glow,
        BM_Phoenix,
        BM_Hue,
        BM_Saturation,
        BM_Color,
        BM_Luminosity,
		BM_None,

        BM_Total
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
