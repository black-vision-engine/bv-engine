#include "stdafx.h"

#include "ModelNodeEffectFactory.h"

#include "Engine/Models/NodeEffects/ModelNodeEffect.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// **************************
//
IModelNodeEffectPtr         CreateDefaultModelNodeEffect            ( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
    return ModelNodeEffect::Create( NodeEffectType::NET_DEFAULT );
}

// **************************
//
IModelNodeEffectPtr         CreateAlphaMaskModelNodeEffect          ( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_ALPHA_MASK );
    
    auto alphaEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    alphaEval->Parameter()->SetVal( 1.f, 0.f );

    effect->RegisterEvaluator( alphaEval );

    return effect;
}

// **************************
//
IModelNodeEffectPtr         CreateNodeMaskModelNodeEffect          ( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
    ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();
    
    h.AddSimpleParam( "alpha", 1.0f, true );
    h.AddSimpleParam( "maskIdx", 0, true );
    h.AddSimpleParam( "fgIdx", 1, true );
    h.AddSimpleParam( "maskChannelIdx", 1, true );
    h.AddSimpleParam( "maskPreview", false, true );
    
    return ModelNodeEffect::Create( NodeEffectType::NET_NODE_MASK, std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() ) );
}

// **************************
//
IModelNodeEffectPtr         CreateColorCorrectionModelNodeEffect( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
    ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    h.AddSimpleParam( "color", glm::vec4( 0, 0, 0, 0 ), true );

    return ModelNodeEffect::Create( NodeEffectType::NET_COLOR_CORRECTION, std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() ) );
}


// **************************
//
IModelNodeEffectPtr         CreateColorBalanceModelNodeEffect( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
    ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    h.AddSimpleParam( "brightness", 1.f, true ); // [0,10](?)
    h.AddSimpleParam( "contrast", 0.f, true ); // [-1,1]
    h.AddSimpleParam( "saturation", 0.f, true ); // [-1,1]
    h.AddSimpleParam( "hue", 0.f, true ); // [0;360]

    return ModelNodeEffect::Create( NodeEffectType::NET_COLOR_BALANCE, std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() ) );
}

// **************************
//
IModelNodeEffectPtr         CreateWireframeModelNodeEffect          ( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
    return ModelNodeEffect::Create( NodeEffectType::NET_WIREFRAME );
}

// **************************
//
IModelNodeEffectPtr         CreateMixchannelsModelNodeEffect        ( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_MIX_CHANNELS );

    auto fgIdxEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "channelMask", timeEvaluator );
    fgIdxEval->Parameter()->SetVal( 0, 0.f );

    effect->RegisterEvaluator( fgIdxEval );

    return effect;
}

// **************************
//
IModelNodeEffectPtr         CreateBlurModelNodeEffect               ( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
	auto effect = ModelNodeEffect::Create( NodeEffectType::NET_BLUR );

	auto blurSize = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );
	auto blurKernelType = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "blurKernelType", timeEvaluator );
	auto normalize = ParamValEvaluatorFactory::CreateSimpleBoolEvaluator( "normalize", timeEvaluator );

	blurSize->Parameter()->SetVal( 0.f, 0.f );
	blurKernelType->Parameter()->SetVal( 0, 0.f );
	normalize->Parameter()->SetVal( 1, 0.f );

	effect->RegisterEvaluator( blurSize );
	effect->RegisterEvaluator( blurKernelType );
	effect->RegisterEvaluator( normalize );

	return effect;
}
//
// **************************
//
IModelNodeEffectPtr         CreateLightScatteringModelNodeEffect    ( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_LIGHT_SCATTERING );

    auto exposureEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "exposure", timeEvaluator );
    auto weightEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "weight", timeEvaluator );
    auto decayEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "decay", timeEvaluator );
    auto densityEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "density", timeEvaluator );
    auto lightPositionOnScreenEval = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "lightPositionOnScreen", timeEvaluator );
    auto numSamplesEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "numSamples", timeEvaluator );

    exposureEval->Parameter()->SetVal( 0.005f, 0.f );
    weightEval->Parameter()->SetVal( 2.65f, 0.f );
    decayEval->Parameter()->SetVal( 1.0f, 0.f );
    densityEval->Parameter()->SetVal( 0.2f, 0.f );
    lightPositionOnScreenEval->Parameter()->SetVal( glm::vec2( 0.1f, 0.4f ), 0.f );
    lightPositionOnScreenEval->Parameter()->SetVal( glm::vec2( 0.4f, 0.6f ), 5.f );
    lightPositionOnScreenEval->Parameter()->SetVal( glm::vec2( 0.5f, 0.4f ), 10.f );
    lightPositionOnScreenEval->Parameter()->SetVal( glm::vec2( 0.4f, 0.1f ), 15.f );
    numSamplesEval->Parameter()->SetVal( 100, 0.f );

    effect->RegisterEvaluator( exposureEval );
    effect->RegisterEvaluator( weightEval );
    effect->RegisterEvaluator( decayEval );
    effect->RegisterEvaluator( densityEval );
    effect->RegisterEvaluator( lightPositionOnScreenEval );
    effect->RegisterEvaluator( numSamplesEval );

    return effect;
}

// **************************
//
IModelNodeEffectPtr         CreateShadowModelNodeEffect    ( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_SHADOW );
 
    auto colorEval = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "color", timeEvaluator );
    auto shiftEval = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "shift", timeEvaluator );
    auto blurSizeEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );
    auto normalizeEval = ParamValEvaluatorFactory::CreateSimpleBoolEvaluator( "normalize", timeEvaluator );
    auto innerEval = ParamValEvaluatorFactory::CreateSimpleBoolEvaluator( "inner", timeEvaluator );
	auto outerEval = ParamValEvaluatorFactory::CreateSimpleBoolEvaluator( "outer", timeEvaluator );
    auto blurKernelTypeEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "blurKernelType", timeEvaluator );

    colorEval->Parameter()->SetVal( glm::vec4( 1.f, 0.f, 0.f, 1.f ), 0.f );
    shiftEval->Parameter()->SetVal( glm::vec2( 0.0f, 0.0f ), 0.f );
    blurSizeEval->Parameter()->SetVal( 5.5f, 0.f );
    blurSizeEval->Parameter()->SetVal( 205.5f, 5.f );
    blurSizeEval->Parameter()->SetVal( 5.5f, 10.f );
    normalizeEval->Parameter()->SetVal( 1, 0.f );
    innerEval->Parameter()->SetVal( 0, false );
	outerEval->Parameter()->SetVal( 0, true );
    blurKernelTypeEval->Parameter()->SetVal( 0, 0.f );

    effect->RegisterEvaluator( colorEval );
    effect->RegisterEvaluator( shiftEval );
    effect->RegisterEvaluator( blurSizeEval );
    effect->RegisterEvaluator( normalizeEval );
    effect->RegisterEvaluator( innerEval );
    effect->RegisterEvaluator( blurKernelTypeEval );
	effect->RegisterEvaluator( outerEval );

    return effect;
}

// **************************
//
IModelNodeEffectPtr         CreateGlowModelNodeEffect    ( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
	auto effect = ModelNodeEffect::Create( NodeEffectType::NET_GLOW );

	auto colorEval = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "color", timeEvaluator );
	auto blurSizeEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );
	auto normalizeEval = ParamValEvaluatorFactory::CreateSimpleBoolEvaluator( "normalize", timeEvaluator );
	auto innerEval = ParamValEvaluatorFactory::CreateSimpleBoolEvaluator( "inner", timeEvaluator );
	auto outerEval = ParamValEvaluatorFactory::CreateSimpleBoolEvaluator( "outer", timeEvaluator );
	auto blurKernelTypeEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "blurKernelType", timeEvaluator );
	auto glowStrengthEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "glowStrength", timeEvaluator );

	colorEval->Parameter()->SetVal( glm::vec4( 1.f, 0.f, 0.f, 1.f ), 0.f );
	blurSizeEval->Parameter()->SetVal( 5.5f, 0.f );
	blurSizeEval->Parameter()->SetVal( 205.5f, 5.f );
	blurSizeEval->Parameter()->SetVal( 5.5f, 10.f );
	normalizeEval->Parameter()->SetVal( 1, 0.f );
	innerEval->Parameter()->SetVal( 0, false );
	outerEval->Parameter()->SetVal( 0, true );
	blurKernelTypeEval->Parameter()->SetVal( 0, 0.f );
	glowStrengthEval->Parameter()->SetVal( 0, 1.f );

	effect->RegisterEvaluator( colorEval );
	effect->RegisterEvaluator( blurSizeEval );
	effect->RegisterEvaluator( normalizeEval );
	effect->RegisterEvaluator( innerEval );
	effect->RegisterEvaluator( blurKernelTypeEval );
	effect->RegisterEvaluator( outerEval );
	effect->RegisterEvaluator( glowStrengthEval );

	return effect;
}

// **************************
//
IModelNodeEffectPtr         CreateSoftMaskModelNodeEffect    ( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
	SimpleTransformEvaluatorPtr maskTxEvaluator = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "maskTx", timeEvaluator );

	ModelHelper h( timeEvaluator );
	h.SetOrCreatePluginModel();
	auto plModel = std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() );

	plModel->RegisterAll( maskTxEvaluator );

	h.AddSimpleParam( "width", 0.01f, true );
	h.AddSimpleParam( "progress", 0.01f, true );
	h.AddSimpleParam( "blankWidth", 0.01f, true );
	h.AddSimpleParam( "invert", false, true );
	h.AddSimpleParam( "alphaOnly", false, true );
	h.AddSimpleParam( "objectOnly", false, true );
	h.AddSimpleParam( "mirrorEnabled", false, true );
	h.AddSimpleParam( "polyDegree", 4, true );

	return ModelNodeEffect::Create( NodeEffectType::NET_SOFT_MASK, plModel );
}

//
//// **************************
////
//IModelNodeEffectPtr         CreateImageMaskModelNodeEffect              ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
//{
//    { name; }
//    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_IMAGE_MASK );
// 
//    auto blendEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blend", timeEvaluator );
//    auto positionEval = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "position", timeEvaluator );
//    auto scaleEval = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "scale", timeEvaluator );
//    auto invertEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "invert", timeEvaluator );
//    auto fitEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "fit", timeEvaluator );
//    auto wrapEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "wrap", timeEvaluator );
//    auto maskAspectEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "maskAspect", timeEvaluator );
//    auto alphaOnlyEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "alphaOnly", timeEvaluator );
//    auto softMaskEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "softMask", timeEvaluator );
//
//    blendEval->Parameter()->SetVal( 1.f, 0.f );
//    positionEval->Parameter()->SetVal( glm::vec2( 0.f, 0.f ), 0.f );
//    scaleEval->Parameter()->SetVal( glm::vec2( 1.f, 1.f ), 0.f );
//    invertEval->Parameter()->SetVal( 0, 0.f );
//    fitEval->Parameter()->SetVal( 1, 0.f );
//    wrapEval->Parameter()->SetVal( 0, 0.f );
//    maskAspectEval->Parameter()->SetVal( 1, 0.f );
//    alphaOnlyEval->Parameter()->SetVal( 0, 0.f );
//    softMaskEval->Parameter()->SetVal( 0, 0.f );
//
//    effect->RegisterEvaluator( blendEval );
//    effect->RegisterEvaluator( positionEval );
//    effect->RegisterEvaluator( scaleEval );
//    effect->RegisterEvaluator( invertEval );
//    effect->RegisterEvaluator( fitEval );
//    effect->RegisterEvaluator( wrapEval );
//    effect->RegisterEvaluator( maskAspectEval );
//    effect->RegisterEvaluator( alphaOnlyEval );
//    effect->RegisterEvaluator( softMaskEval );
//
//    return effect;
//}
//
//// **************************
////
//IModelNodeEffectPtr         CreateBoundingBoxModelNodeEffect          ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
//{
//    { name; }
//    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_BOUNDING_BOX );
//
//    auto color = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "color", timeEvaluator );
//
//    color->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 1.f, 1.f ), 0.f );
//
//    effect->RegisterEvaluator( color );
//
//    return effect;
//}

// ***********************
//
IModelNodeEffectPtr         CreateZSortModelNodeEffect                  ( const std::string &, ITimeEvaluatorPtr timeEvaluator )
{
    ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    h.AddSimpleParam( "useSort", false, true, false );

    return ModelNodeEffect::Create( NodeEffectType::NET_Z_SORT, std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() ) );
}


// ********************************
//
IModelNodeEffectPtr         ModelNodeEffectFactory::CreateModelNodeEffect     ( NodeEffectType nodeEffectType, const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    IModelNodeEffectPtr ret = nullptr;

    switch( nodeEffectType )
    {
        case NodeEffectType::NET_DEFAULT:
            ret = CreateDefaultModelNodeEffect( name, timeEvaluator );
            break;
        case NodeEffectType::NET_ALPHA_MASK:
            ret = CreateAlphaMaskModelNodeEffect( name, timeEvaluator );
            break;
        case NodeEffectType::NET_NODE_MASK:
            ret = CreateNodeMaskModelNodeEffect( name, timeEvaluator );
            break;
        case NodeEffectType::NET_WIREFRAME:
            ret = CreateWireframeModelNodeEffect( name, timeEvaluator );
            break;
        case NodeEffectType::NET_MIX_CHANNELS:
            ret = CreateMixchannelsModelNodeEffect( name, timeEvaluator );
            break;
        case NodeEffectType::NET_BLUR:
            ret = CreateBlurModelNodeEffect( name, timeEvaluator );
            break;
        case NodeEffectType::NET_LIGHT_SCATTERING:
            ret = CreateLightScatteringModelNodeEffect( name, timeEvaluator );
            break;
        case NodeEffectType::NET_SHADOW:
            ret = CreateShadowModelNodeEffect( name, timeEvaluator );
            break;
		case NodeEffectType::NET_GLOW:
            ret = CreateGlowModelNodeEffect( name, timeEvaluator );
            break;
        case NodeEffectType::NET_SOFT_MASK:
            ret = CreateSoftMaskModelNodeEffect( name, timeEvaluator );
            break;
        //case NodeEffectType::NET_BOUNDING_BOX:
        //    return CreateBoundingBoxModelNodeEffect( name, timeEvaluator );
        //case NodeEffectType::NET_IMAGE_MASK:
        //    return CreateImageMaskModelNodeEffect( name, timeEvaluator );
        case NodeEffectType::NET_Z_SORT:
            ret = CreateZSortModelNodeEffect( name, timeEvaluator );
            break;
        case NodeEffectType::NET_COLOR_CORRECTION:
            ret = CreateColorCorrectionModelNodeEffect( name, timeEvaluator );
            break;
        case NodeEffectType::NET_COLOR_BALANCE:
            ret = CreateColorBalanceModelNodeEffect( name, timeEvaluator );
            break;
        default:
            assert( false );
    }

    if( ret )
    {
        auto typedModelNodeEffect = std::static_pointer_cast< ModelNodeEffect >( ret );

        if( !typedModelNodeEffect->GetParameter( ModelNodeEffect::EFFECT_ENABLED_PARAM_NAME ) )
        {
            auto enabledParamEval = ParamValEvaluatorFactory::CreateSimpleBoolEvaluator( ModelNodeEffect::EFFECT_ENABLED_PARAM_NAME, timeEvaluator );
            enabledParamEval->Parameter()->SetVal( true, 0.f );
            ret->RegisterEvaluator( enabledParamEval );
        }
    }

    return ret;
}

std::string                 ModelNodeEffectFactory::QueryGizmoName          ( NodeEffectType nodeEffectType, const std::string & function )
{
    nodeEffectType;
    function;

    // TODO: Here place mapping of node effects gizmos.

    return std::string();
}

} // model
} // bv
