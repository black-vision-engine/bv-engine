#include "ModelNodeEffectWireframe.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"


namespace bv { namespace model {


// ********************************
//
ModelNodeEffectWireframe::ModelNodeEffectWireframe  ( ITimeEvaluatorPtr timeEvaluator )
{ 
    { timeEvaluator; }
}

// ********************************
//
void            ModelNodeEffectWireframe::Update    ( TimeType t )
{
    { t; }
}

// ********************************
//
NodeEffectType  ModelNodeEffectWireframe::GetType   () const
{
    return NodeEffectType::NET_WIREFRAME;
}

} // model
} // bv
