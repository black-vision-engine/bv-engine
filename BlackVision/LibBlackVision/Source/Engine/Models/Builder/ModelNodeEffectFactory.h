#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"

namespace bv
{
namespace model
{

class ModelNodeEffectFactory
{
public:
    ModelNodeEffectFactory();
    ~ModelNodeEffectFactory();

    static IModelNodeEffectPtr     Create       ( const std::string& effectName, ITimeEvaluatorPtr timeEvaluator );
    static IModelNodeEffectPtr     Create       ( NodeEffectType effectType, ITimeEvaluatorPtr timeEvaluator );
};


} // model
} // bv
