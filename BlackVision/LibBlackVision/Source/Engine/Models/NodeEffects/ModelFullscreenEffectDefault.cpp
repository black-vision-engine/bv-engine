#include "ModelFullscreenEffectDefault.h"


namespace bv { namespace model {


// ********************************
//
ModelFullscreenEffectDefault::ModelFullscreenEffectDefault  ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
    : ModelSimpleFullscreenEffect( name )
{ 
    { timeEvaluator; }
}

// ********************************
//
NodeEffectType    ModelFullscreenEffectDefault::GetType   () const
{
    return NodeEffectType::NET_DEFAULT;
}

} // model
} // bv
