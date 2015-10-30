#include "ModelNodeEffectWireframe.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"


namespace bv { namespace model {


// ********************************
//
ModelNodeEffectWireframe::ModelNodeEffectWireframe( ITimeEvaluatorPtr timeEvaluator )
{ 
    { timeEvaluator; }
}

// ********************************
//
NodeEffectType  ModelNodeEffectWireframe::GetType() const
{
    return NodeEffectType::NET_WIREFRAME;
}

} // model
} // bv
