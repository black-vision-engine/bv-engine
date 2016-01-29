#include "stdafx.h"

#include "ModelNodeEffectBlur.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"

namespace bv { namespace model {


// ********************************
//
ModelNodeEffectBlur::ModelNodeEffectBlur( ITimeEvaluatorPtr timeEvaluator )
	: ModelNodeEffectBase( timeEvaluator )
	//, m_blurSizeVal( 5.0f )
	, m_blurSizeVal( 5.5f )
    , m_normalizeVal( 1 )
    , m_blurKernelType( 1 )
{ 
	auto blurSizeEval = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );
    auto normalizeEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "normalize", timeEvaluator );
    auto blurKernelTypeEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "blurKernelType", timeEvaluator );

    blurSizeEval->Parameter()->SetVal( m_blurSizeVal, 0.f );
    normalizeEval->Parameter()->SetVal( m_normalizeVal, 0.f );
    blurKernelTypeEval->Parameter()->SetVal( m_blurKernelType, 0.f );

	m_paramValModel->RegisterAll( blurSizeEval );
    m_paramValModel->RegisterAll( normalizeEval );
    m_paramValModel->RegisterAll( blurKernelTypeEval );

	m_paramBlurSize = blurSizeEval->Parameter();
    m_paramNormalize = normalizeEval->Parameter();
    m_paramBlurKernelType = blurKernelTypeEval->Parameter();
}

// ********************************
//
NodeEffectType  ModelNodeEffectBlur::GetType				    () const
{
    return NodeEffectType::NET_BLUR;
}

//// ********************************
////
//ParamFloatPtr   ModelNodeEffectBlur::GetParamBlurSize         () const
//{
//    return m_paramBlurSize;
//}

// ********************************
//
void            ModelNodeEffectBlur::Update                   ( TimeType t )
{
	ModelNodeEffectBase::Update( t );

	m_paramValModel->Update();
	
	m_blurSizeVal = m_paramBlurSize->Evaluate();
}

//// ********************************
////
//Float32           ModelNodeEffectBlur::GetBlurSize              () const
//{
//    return m_blurSizeVal;
//}
//
//// ********************************
////
//ParamIntPtr     ModelNodeEffectBlur::GetParamNormalize       () const
//{
//    return m_paramNormalize;
//}
//
//// ********************************
////
//Int32           ModelNodeEffectBlur::GetNormalize            () const
//{
//    return m_normalizeVal;
//}


} // model
} // bv
