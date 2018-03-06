#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.inl"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"



namespace bv {
namespace model {



class BlendHelper
{
public:

	struct PARAM
	{
        static const std::string        BLEND_MODE;

		static const std::string		BLEND_ENABLE;
		static const std::string        COLOR_BLEND_MODE;
        static const std::string        ALPHA_BLEND_MODE;
	};

    enum BlendMode
    {
        BM_Normal = 0,
        BM_Lighten,			// Blending function GL_MAX needed.
        BM_Darken,			// Blending function GL_MIN needed.
        BM_Multiply,
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
        BM_Alpha,

        BM_Total
    };



public:
	static void         SetBlendRendererContext		( DefaultPixelShaderChannelPtr psc, const SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM > & param );
    static void         SetBlendRendererContext		( DefaultPixelShaderChannelPtr psc, const SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM > & colorParam, const SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM > & alphaParam );
    
    static void         SetBlendRendererContext		( DefaultPixelShaderChannelPtr psc, const ParamEnum< BlendHelper::BlendMode > * param );
    static void         SetBlendRendererContext		( DefaultPixelShaderChannelPtr psc, const ParamEnum< BlendHelper::BlendMode > * colorBlendParam, const ParamEnum< BlendHelper::BlendMode > * alphaBlendParam );

    static void         SetBlendColorContext		( RendererContextPtr & ctx, const ParamEnum< BlendHelper::BlendMode > * param );
    static void         SetBlendAlphaContext		( RendererContextPtr & ctx, const ParamEnum< BlendHelper::BlendMode > * param );

    static void         SetBlendColorContext		( RendererContextPtr & ctx, BlendHelper::BlendMode mode );
    static void         SetBlendAlphaContext		( RendererContextPtr & ctx, BlendHelper::BlendMode mode );

	static void			UpdateBlendState			( DefaultPixelShaderChannelPtr psc, ValueParamState< bool > & blenEnable, ValueParamState< BlendHelper::BlendMode > & blendMode );
    static void			UpdateBlendState			( DefaultPixelShaderChannelPtr psc, ValueParamState< bool > & blenEnable, ValueParamState< BlendHelper::BlendMode > & colorBlendMode, ValueParamState< BlendHelper::BlendMode > & alphaBlendMode );

    static BlendMode    ContextToColorBlendMode     ( RendererContextConstPtr & ctx );
    static BlendMode    ContextToAlphaBlendMode     ( RendererContextConstPtr & ctx );
};


typedef ParamEnum< BlendHelper::BlendMode > ParamEnumBlendMode;


DEFINE_ENUM_SET_PARAMETER( BlendHelper::BlendMode );


// ***********************
//
inline VoidPtr    ParamEnumBlendMode::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
inline static IParameterPtr        ParametersFactory::CreateTypedParameter< BlendHelper::BlendMode >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< BlendHelper::BlendMode >( name, timeline );
}



}   // model
}	// bv
