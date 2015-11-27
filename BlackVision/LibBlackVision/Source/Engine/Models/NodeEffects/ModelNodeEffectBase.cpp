#include "ModelNodeEffectBase.h"


namespace bv { namespace model {

// ********************************
//
ModelNodeEffectBase::ModelNodeEffectBase  ( ITimeEvaluatorPtr timeEvaluator )
	: m_paramValModel( std::make_shared< DefaultParamValModel >() )
{
    { timeEvaluator; }
}

// ********************************
//
void            ModelNodeEffectBase::Update  ( TimeType t )
{
    { t; }
	m_paramValModel->Update();
}

// ********************************
//
const std::vector< bv::IValueConstPtr > &	ModelNodeEffectBase::GetValues () const
{
    return m_paramValModel->GetValues();
}

} // model
} // bv
