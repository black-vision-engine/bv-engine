#include "ModelFullscreenEffectWireframe.h"


namespace bv { namespace model {


// ********************************
//
ModelFullscreenEffectWireframe::ModelFullscreenEffectWireframe  ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
    : ModelSimpleFullscreenEffect( name )
{ 
    { timeEvaluator; }
}

} // model
} // bv
