#include "stdafx.h"

#include "ModelNodeEffectNodeMask.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"

namespace bv { namespace model {


// ********************************
//
ModelNodeEffectNodeMask::ModelNodeEffectNodeMask( ITimeEvaluatorPtr timeEvaluator )
	: ModelNodeEffectBase( timeEvaluator )
	, m_bgVal( 0 )
	, m_fgVal( 1 )
	, m_alphaVal( 1.f )
{ 
	auto bgIdxEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "bgIdx", timeEvaluator );
	auto fgIdxEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "fgIdx", timeEvaluator );
	auto alphaEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );

    bgIdxEval->Parameter()->SetVal( m_bgVal, 0.f );
    fgIdxEval->Parameter()->SetVal( m_fgVal, 0.f );
    alphaEval->Parameter()->SetVal( m_alphaVal, 0.f );

	m_paramValModel->RegisterAll( bgIdxEval );
	m_paramValModel->RegisterAll( fgIdxEval );
	m_paramValModel->RegisterAll( alphaEval );

	m_paramBgIdx = bgIdxEval->Parameter();
	m_paramFgIdx = fgIdxEval->Parameter();
	m_paramAlpha = alphaEval->Parameter();
}

// ********************************
//
NodeEffectType  ModelNodeEffectNodeMask::GetType				() const
{
    return NodeEffectType::NET_NODE_MASK;
}

// ********************************
//
ParamIntPtr     ModelNodeEffectNodeMask::GetParamBgIdx           () const
{
    return m_paramBgIdx;
}

// ********************************
//
ParamIntPtr     ModelNodeEffectNodeMask::GetParamFgIdx           () const
{
    return m_paramFgIdx;
}

// ********************************
//
ParamFloatPtr   ModelNodeEffectNodeMask::GetParamAlpha           () const
{
    return m_paramAlpha;
}

// ********************************
//
void            ModelNodeEffectNodeMask::Update  ( TimeType t )
{
	ModelNodeEffectBase::Update( t );

	m_paramValModel->Update();
	
	m_bgVal = m_paramBgIdx->Evaluate();
	m_fgVal = m_paramFgIdx->Evaluate();
	m_alphaVal = m_paramAlpha->Evaluate();

    assert( m_bgVal != m_fgVal );
}

// ********************************
//
unsigned int    ModelNodeEffectNodeMask::GetBackgroundChildIdx   () const
{
    return m_bgVal;
}

// ********************************
//
unsigned int    ModelNodeEffectNodeMask::GetForegroundChildIdx   () const
{
    return m_fgVal;
}

// ********************************
//
float           ModelNodeEffectNodeMask::GetAlpha                () const
{
    return m_alphaVal;
}

} // model
} // bv
