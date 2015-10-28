#include "ModelNodeEffectDefault.h"


namespace bv { namespace model {

// ********************************
//
ModelNodeEffectDefault::ModelNodeEffectDefault  ( ITimeEvaluatorPtr timeEvaluator )
{
    { timeEvaluator; }
}

// ********************************
//
NodeEffectType  ModelNodeEffectDefault::GetType () const
{
    return NodeEffectType::NET_DEFAULT;
}

// ********************************
//
void            ModelNodeEffectDefault::Update  ( TimeType t )
{
    { t; }
}

} // model
} // bv
