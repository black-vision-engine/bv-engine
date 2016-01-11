#include "ModelFullscreenEffectWireframe.h"


namespace bv { namespace model {


// ********************************
//
ModelFullscreenEffectWireframe::ModelFullscreenEffectWireframe  ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
    : ModelSimpleFullscreenEffect( name )
{ 
    { timeEvaluator; }
}

// ********************************
//
NodeEffectType    ModelFullscreenEffectWireframe::GetType   () const
{
    return NodeEffectType::NET_WIREFRAME;
}

} // model
} // bv
