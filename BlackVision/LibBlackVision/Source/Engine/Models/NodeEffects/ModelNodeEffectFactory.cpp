#include "ModelNodeEffectFactory.h"

#include "Engine/Models/NodeEffects/ModelNodeEffect.h"
#include "Engine/Models/NodeEffects/ModelFullscreenEffectDefault.h"
#include "Engine/Models/NodeEffects/ModelFullscreenEffectAlphaMask.h"
#include "Engine/Models/NodeEffects/ModelFullscreenEffectNodeMask.h"
#include "Engine/Models/NodeEffects/ModelFullscreenEffectWireframe.h"
#include "Engine/Models/NodeEffects/ModelFullscreenEffectMixChannels.h"


namespace bv { namespace model {


// ********************************
//
IModelNodeEffectPtr         ModelNodeEffectFactory::CreateModelNodeEffect     ( NodeEffectType nodeEffectType, const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    IModelFullscreenEffectPtr fse = nullptr;

    switch( nodeEffectType )
    {
        case  NodeEffectType::NET_DEFAULT:

            fse = std::make_shared< ModelFullscreenEffectDefault >( name, timeEvaluator );
            break;
        case NodeEffectType::NET_ALPHA_MASK:
            fse =  std::make_shared< ModelFullscreenEffectAlphaMask >( name, timeEvaluator );
            break;
        case NodeEffectType::NET_NODE_MASK:
            fse =  std::make_shared< ModelFullscreenEffectNodeMask >( name, timeEvaluator );
            break;
        case NodeEffectType::NET_WIREFRAME:
            fse =  std::make_shared< ModelFullscreenEffectWireframe >( name, timeEvaluator );
            break;
        case NodeEffectType::NET_MIX_CHANNELS:
            fse =  std::make_shared< ModelFullscreenEffectMixChannels >( name, timeEvaluator );
            break;
        default:
            assert( false );
    }

    return ModelNodeEffect::Create( fse );
}

} // model
} // bv
