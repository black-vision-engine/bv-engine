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
        default:
            assert( false );
    }

    return nullptr;
}

} // model
} // bv
