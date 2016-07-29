#include "ModelFullscreenEffectDefault.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


// ********************************
//
ModelFullscreenEffectDefault::ModelFullscreenEffectDefault  ( const std::string & name, ITimeEvaluatorPtr timeEvaluator )
    : ModelSimpleFullscreenEffect( name )
{ 
    { timeEvaluator; }
}

} // model
} // bv
