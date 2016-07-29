#include "ModelFullscreenEffectWireframe.h"




#include "Memory/MemoryLeaks.h"



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
