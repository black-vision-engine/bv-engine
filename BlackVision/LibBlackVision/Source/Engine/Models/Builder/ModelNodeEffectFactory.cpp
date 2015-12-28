#include "ModelNodeEffectFactory.h"
#include "Serialization/SerializationHelper.h"

#include "Engine/Models/NodeEffects/ModelNodeEffectAlphaMask.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectDefault.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectLightScattering.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectNodeMask.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectWireframe.h"

namespace bv
{
namespace model
{

// ***********************
//
ModelNodeEffectFactory::ModelNodeEffectFactory()
{}
// ***********************
//
ModelNodeEffectFactory::~ModelNodeEffectFactory()
{}

// ***********************
//
IModelNodeEffectPtr     ModelNodeEffectFactory::Create       ( const std::string& effectName, ITimeEvaluatorPtr timeEvaluator )
{
    NodeEffectType effectType = SerializationHelper::String2T<NodeEffectType>( effectName, NodeEffectType::NET_DEFAULT );
    return Create( effectType, timeEvaluator );
}

// ***********************
//
IModelNodeEffectPtr     ModelNodeEffectFactory::Create       ( NodeEffectType effectType, ITimeEvaluatorPtr timeEvaluator )
{
    switch ( effectType )
    {
    case NodeEffectType::NET_ALPHA_MASK:
        return std::make_shared<ModelNodeEffectAlphaMask>( timeEvaluator );
        break;
    case NodeEffectType::NET_DEFAULT:
        return std::make_shared<ModelNodeEffectDefault>( timeEvaluator );
        break;
    case NodeEffectType::NET_LIGHT_SCATTERING:
        return std::make_shared<ModelNodeEffectLightScattering>( timeEvaluator );
        break;
    case NodeEffectType::NET_NODE_MASK:
        return std::make_shared<ModelNodeEffectNodeMask>( timeEvaluator );
        break;
    case NodeEffectType::NET_WIREFRAME:
        return std::make_shared<ModelNodeEffectWireframe>( timeEvaluator );
        break;
    default:
        return nullptr;
    }

}



} // model
} // bv
