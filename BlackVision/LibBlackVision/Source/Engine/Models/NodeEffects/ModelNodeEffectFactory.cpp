#include "stdafx.h"

#include "ModelNodeEffectFactory.h"

#include "Engine/Models/NodeEffects/ModelNodeEffect.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"


namespace bv { namespace model {

// **************************
//
IModelNodeEffectPtr         CreateDefaultModelNodeEffect            ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    { name; }
    return ModelNodeEffect::Create( NodeEffectType::NET_DEFAULT );
}

// **************************
//
IModelNodeEffectPtr         CreateAlphaMaskModelNodeEffect          ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    { name; }
    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_ALPHA_MASK );
    
    auto alphaEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    alphaEval->Parameter()->SetVal( 1.f, 0.f );

    effect->RegisterEvaluator( alphaEval );

    return effect;
}

// **************************
//
IModelNodeEffectPtr         CreateNodeMaskModelNodeEffect          ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    { name; }
    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_NODE_MASK );

    auto alphaEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    auto maskIdxEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "maskIdx", timeEvaluator );
    auto fgIdxEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "fgIdx", timeEvaluator );

    alphaEval->Parameter()->SetVal( 1.f, 0.f );
    maskIdxEval->Parameter()->SetVal( 0, 0.f );
    fgIdxEval->Parameter()->SetVal( 1, 0.f );

    effect->RegisterEvaluator( alphaEval );
    effect->RegisterEvaluator( maskIdxEval );
    effect->RegisterEvaluator( fgIdxEval );
    
    return effect;
}

// **************************
//
IModelNodeEffectPtr         CreateWireframeModelNodeEffect          ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    { name; }
    return ModelNodeEffect::Create( NodeEffectType::NET_WIREFRAME );
}

// **************************
//
IModelNodeEffectPtr         CreateMixchannelsModelNodeEffect        ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    { name; }
    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_MIX_CHANNELS );

    auto fgIdxEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "channelMask", timeEvaluator );
    fgIdxEval->Parameter()->SetVal( 0, 0.f );

    effect->RegisterEvaluator( fgIdxEval );

    return effect;
}

// **************************
//
IModelNodeEffectPtr         CreateBlurModelNodeEffect               ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    { name; }
    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_BLUR );

    auto blurSizeEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );
    auto normalizeEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "normalize", timeEvaluator );
    auto blurKernelTypeEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "blurKernelType", timeEvaluator );
    
    blurSizeEval->Parameter()->SetVal( 190.5f, 0.f );
    blurSizeEval->Parameter()->SetVal( 1.5f, 19.f );
    normalizeEval->Parameter()->SetVal( 1, 0.f );
    blurKernelTypeEval->Parameter()->SetVal( 2, 0.f );

    effect->RegisterEvaluator( blurSizeEval );
    effect->RegisterEvaluator( normalizeEval );
    effect->RegisterEvaluator( blurKernelTypeEval );

    return effect;
}

// **************************
//
IModelNodeEffectPtr         CreateLightScatteringModelNodeEffect    ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    { name; }
    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_LIGHT_SCATTERING );

    auto exposureEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "exposure", timeEvaluator );
    auto weightEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "weight", timeEvaluator );
    auto decayEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "decay", timeEvaluator );
    auto densityEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "density", timeEvaluator );
    auto lightPositionOnScreenEval = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "lightPositionOnScreen", timeEvaluator );
    auto numSamplesEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "numSamples", timeEvaluator );

    exposureEval->Parameter()->SetVal( 0.005f, 0.f );
    weightEval->Parameter()->SetVal( 2.65f, 0.f );
    decayEval->Parameter()->SetVal( 1.0f, 0.f );
    densityEval->Parameter()->SetVal( 0.2f, 0.f );
    lightPositionOnScreenEval->Parameter()->SetVal( glm::vec2( 0.1f, 0.4f ), 0.f );
    lightPositionOnScreenEval->Parameter()->SetVal( glm::vec2( 0.4f, 0.6f ), 5.f );
    lightPositionOnScreenEval->Parameter()->SetVal( glm::vec2( 0.5f, 0.4f ), 10.f );
    lightPositionOnScreenEval->Parameter()->SetVal( glm::vec2( 0.4f, 0.1f ), 15.f );
    numSamplesEval->Parameter()->SetVal( 100.f, 0.f );

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
IModelNodeEffectPtr         CreateShadowModelNodeEffect    ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    { name; }
    auto effect = ModelNodeEffect::Create( NodeEffectType::NET_SHADOW );
 
    auto colorEval = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "color", timeEvaluator );
    auto shiftEval = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "shift", timeEvaluator );
    auto blurSizeEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );
    auto normalizeEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "normalize", timeEvaluator );
    auto innerEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "inner", timeEvaluator );

    colorEval->Parameter()->SetVal( glm::vec4( 1.f, 0.f, 0.f, 1.f ), 0.f );
    shiftEval->Parameter()->SetVal( glm::vec2( 0.f, 0.f ), 0.f );
    blurSizeEval->Parameter()->SetVal( 5.5f, 0.f );
    normalizeEval->Parameter()->SetVal( 1, 0.f );
    innerEval->Parameter()->SetVal( 0, 0.f );

    effect->RegisterEvaluator( colorEval );
    effect->RegisterEvaluator( shiftEval );
    effect->RegisterEvaluator( blurSizeEval );
    effect->RegisterEvaluator( normalizeEval );
    effect->RegisterEvaluator( innerEval );

    return effect;
}

// **************************
//
IModelNodeEffectPtr         CreateBoundingBoxModelNodeEffect          ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    { name; }
    return ModelNodeEffect::Create( NodeEffectType::NET_BOUNDING_BOX );
}

// ********************************
//
IModelNodeEffectPtr         ModelNodeEffectFactory::CreateModelNodeEffect     ( NodeEffectType nodeEffectType, const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    switch( nodeEffectType )
    {
        case  NodeEffectType::NET_DEFAULT:
            return CreateDefaultModelNodeEffect( name, timeEvaluator );
        case NodeEffectType::NET_ALPHA_MASK:
            return CreateAlphaMaskModelNodeEffect( name, timeEvaluator );
        case NodeEffectType::NET_NODE_MASK:
            return CreateNodeMaskModelNodeEffect( name, timeEvaluator );
        case NodeEffectType::NET_WIREFRAME:
            return CreateWireframeModelNodeEffect( name, timeEvaluator );
        case NodeEffectType::NET_MIX_CHANNELS:
            return CreateMixchannelsModelNodeEffect( name, timeEvaluator );
        case NodeEffectType::NET_BLUR:
            return CreateBlurModelNodeEffect( name, timeEvaluator );
        case NodeEffectType::NET_LIGHT_SCATTERING:
            return CreateLightScatteringModelNodeEffect( name, timeEvaluator );
        case NodeEffectType::NET_SHADOW:
            return CreateShadowModelNodeEffect( name, timeEvaluator );
        case NodeEffectType::NET_BOUNDING_BOX:
            return CreateBoundingBoxModelNodeEffect( name, timeEvaluator );
        default:
            assert( false );
    }

    return nullptr;
}

} // model
} // bv
