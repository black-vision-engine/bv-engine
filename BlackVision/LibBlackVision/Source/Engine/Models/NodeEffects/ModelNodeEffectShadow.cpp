#include "ModelNodeEffectShadow.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"

namespace bv { namespace model {


// ********************************
//
ModelNodeEffectShadow::ModelNodeEffectShadow( ITimeEvaluatorPtr timeEvaluator )
	: ModelNodeEffectBase( timeEvaluator )
	, m_colorVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ) )
	, m_shiftVal( glm::vec2( 0.f, 0.f ) )
	, m_blurSizeVal( 0.01f )
{ 
	auto colorEval = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "color", timeEvaluator );
	auto shiftEval = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "shift", timeEvaluator );
	auto blurSizeEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );

    colorEval->Parameter()->SetVal( m_colorVal, 0.f );
    shiftEval->Parameter()->SetVal( m_shiftVal, 0.f );
    blurSizeEval->Parameter()->SetVal( m_blurSizeVal, 0.f );

	m_paramValModel->RegisterAll( colorEval );
	m_paramValModel->RegisterAll( shiftEval );
	m_paramValModel->RegisterAll( blurSizeEval );

	m_paramColor = colorEval->Parameter();
	m_paramShift = shiftEval->Parameter();
	m_paramBlurSize = blurSizeEval->Parameter();
}

// ********************************
//
NodeEffectType  ModelNodeEffectShadow::GetType				    () const
{
    return NodeEffectType::NET_SHADOW;
}

// ********************************
//
ParamVec4Ptr   ModelNodeEffectShadow::GetParamColor             () const
{
    return m_paramColor;
}

// ********************************
//
ParamVec2Ptr   ModelNodeEffectShadow::GetParamShift             () const
{
    return m_paramShift;
}

// ********************************
//
ParamFloatPtr   ModelNodeEffectShadow::GetParamBlurSize         () const
{
    return m_paramBlurSize;
}

// ********************************
//
void            ModelNodeEffectShadow::Update                   ( TimeType t )
{
	ModelNodeEffectBase::Update( t );

	m_paramValModel->Update();
	
	m_colorVal = m_paramColor->Evaluate();
	m_shiftVal = m_paramShift->Evaluate();
	m_blurSizeVal = m_paramBlurSize->Evaluate();
}

// ********************************
//
glm::vec4       ModelNodeEffectShadow::GetColor                 () const
{
    return m_colorVal;
}

// ********************************
//
glm::vec2       ModelNodeEffectShadow::GetShift                 () const
{
    return m_shiftVal;
}

// ********************************
//
float           ModelNodeEffectShadow::GetBlurSize              () const
{
    return m_blurSizeVal;
}

} // model
} // bv
