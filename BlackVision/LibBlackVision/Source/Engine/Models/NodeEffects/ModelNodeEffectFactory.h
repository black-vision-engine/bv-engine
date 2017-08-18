#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class ModelNodeEffectFactory
{
public:

    static IModelNodeEffectPtr      CreateModelNodeEffect           ( NodeEffectType nodeEffectType, const std::string & name, ITimeEvaluatorPtr timeEvaluator );
    static std::string              QueryGizmoName                  ( NodeEffectType nodeEffectType, const std::string & function );
};

} // model
} // bv
