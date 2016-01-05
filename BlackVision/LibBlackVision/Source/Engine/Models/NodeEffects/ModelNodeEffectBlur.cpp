#include "ModelNodeEffectBlur.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"

namespace bv { namespace model {


// ********************************
//
ModelNodeEffectBlur::ModelNodeEffectBlur( ITimeEvaluatorPtr timeEvaluator )
	: ModelNodeEffectBase( timeEvaluator )
	//, m_blurSizeVal( 5.0f )
	, m_blurSizeVal( 5.0f )
{ 
	auto blurSizeEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );

    blurSizeEval->Parameter()->SetVal( m_blurSizeVal, 0.f );

	m_paramValModel->RegisterAll( blurSizeEval );

	m_paramBlurSize = blurSizeEval->Parameter();
}

// ********************************
//
NodeEffectType  ModelNodeEffectBlur::GetType				    () const
{
    return NodeEffectType::NET_BLUR;
}

// ********************************
//
ParamFloatPtr   ModelNodeEffectBlur::GetParamBlurSize         () const
{
    return m_paramBlurSize;
}

// ********************************
//
void            ModelNodeEffectBlur::Update                   ( TimeType t )
{
	ModelNodeEffectBase::Update( t );

	m_paramValModel->Update();
	
	m_blurSizeVal = m_paramBlurSize->Evaluate();
}

// ********************************
//
float           ModelNodeEffectBlur::GetBlurSize              () const
{
    return m_blurSizeVal;
}

} // model
} // bv
