#include "SamplerStateModel.h"

#include "Engine/Types/Values/BaseValue.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"

namespace bv { namespace model {

// **************************
//
const std::string SamplerStateModel::Params::WRAPPING_MODE_X = "wrapModeX";
const std::string SamplerStateModel::Params::WRAPPING_MODE_Y = "wrapModeY";
const std::string SamplerStateModel::Params::WRAPPING_MODE_Z = "wrapModeZ";
const std::string SamplerStateModel::Params::FILTERING_MODE = "filteringMode";
const std::string SamplerStateModel::Params::BORDER_COLOR = "borderColor";

// **************************
//
SamplerStateModel::SamplerStateModel	( ITimeEvaluatorPtr timeEvaluator )
{
	Initialize( timeEvaluator, TextureWrappingMode::TWM_CLAMP_BORDER, TextureWrappingMode::TWM_CLAMP_BORDER, TextureWrappingMode::TWM_CLAMP_BORDER, 
		TextureFilteringMode::TFM_LINEAR, glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
}

// **************************
//
SamplerStateModel::SamplerStateModel	( ITimeEvaluatorPtr timeEvaluator, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureWrappingMode wmz, TextureFilteringMode fm, const glm::vec4 & bc )
{
	Initialize( timeEvaluator, wmx, wmy, wmz, fm, bc );
}

// **************************
//
SamplerStateModelPtr			SamplerStateModel::Create				( ITimeEvaluatorPtr timeEvaluator )
{
	return std::make_shared< SamplerStateModel >( timeEvaluator );
}

// **************************
//
SamplerStateModelPtr			SamplerStateModel::Create				( ITimeEvaluatorPtr timeEvaluator, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureWrappingMode wmz, TextureFilteringMode fm, const glm::vec4 & bc )
{
	return std::make_shared< SamplerStateModel >( timeEvaluator, wmx, wmy, wmz, fm, bc );
}

// **************************
//
TextureWrappingMode				SamplerStateModel::GetWrappingModeX		() const
{
	return static_cast< TextureWrappingMode >( m_wrappingModeX->GetValue() );
}

// **************************
//
TextureWrappingMode				SamplerStateModel::GetWrappingModeY		() const
{
	return static_cast< TextureWrappingMode >( m_wrappingModeY->GetValue() );
}

// **************************
//
TextureWrappingMode				SamplerStateModel::GetWrappingModeZ		() const
{
	return static_cast< TextureWrappingMode >( m_wrappingModeZ->GetValue() );
}

// **************************
//
TextureFilteringMode			SamplerStateModel::GetFilteringMode		() const
{
	return static_cast< TextureFilteringMode >( m_filteringMode->GetValue() );
}

// **************************
//
const glm::vec4 &				SamplerStateModel::GetBorderColor		() const
{
	return m_borderColor->GetValue();
}

// **************************
//
void					SamplerStateModel::SetWrappingModeX		( TextureWrappingMode wm )
{
	return m_wrappingModeX->SetValue( static_cast< int >( wm ) );
}

// **************************
//
void					SamplerStateModel::SetWrappingModeY		( TextureWrappingMode wm )
{
	return m_wrappingModeY->SetValue( static_cast< int >( wm ) );
}

// **************************
//
void					SamplerStateModel::SetWrappingModeZ		( TextureWrappingMode wm )
{
	return m_wrappingModeZ->SetValue( static_cast< int >( wm ) );
}

// **************************
//
void					SamplerStateModel::SetFilteringMode		( TextureFilteringMode fm )
{
	return m_filteringMode->SetValue( static_cast< int >( fm ) );
}

// **************************
//
void					SamplerStateModel::SetBorderColor		( const glm::vec4 & bc )
{
	return m_borderColor->SetValue( bc );
}

// **************************
//
void					SamplerStateModel::Initialize			( ITimeEvaluatorPtr timeEvaluator, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureWrappingMode wmz, TextureFilteringMode fm, const glm::vec4 & bc )
{
	//FIXME: does better api for this exists?
	auto wmxEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( Params::WRAPPING_MODE_X, timeEvaluator );
	auto wmyEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( Params::WRAPPING_MODE_Y, timeEvaluator );
	auto wmzEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( Params::WRAPPING_MODE_Z, timeEvaluator );
	auto fmEval = ParamValEvaluatorFactory::CreateSimpleIntEvaluator( Params::FILTERING_MODE, timeEvaluator );
	auto bcEval = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( Params::BORDER_COLOR, timeEvaluator );

	RegisterAll( wmxEval );
	RegisterAll( wmyEval );
	RegisterAll( wmzEval );
	RegisterAll( fmEval );
	RegisterAll( bcEval );

	wmxEval->Parameter()->SetVal( static_cast< int >( wmx ), TimeType( 0.f ) );
	wmyEval->Parameter()->SetVal( static_cast< int >( wmy ), TimeType( 0.f ) );
	wmzEval->Parameter()->SetVal( static_cast< int >( wmz ), TimeType( 0.f ) );
	fmEval->Parameter()->SetVal( static_cast< int >( fm ), TimeType( 0.f ) );
	bcEval->Parameter()->SetVal( static_cast< glm::vec4 >( bc ), TimeType( 0.f ) );

	m_wrappingModeX = QueryTypedValue< ValueIntPtr >( GetValue( Params::WRAPPING_MODE_X ) );
	m_wrappingModeY = QueryTypedValue< ValueIntPtr >( GetValue( Params::WRAPPING_MODE_Y ) );
	m_wrappingModeZ = QueryTypedValue< ValueIntPtr >( GetValue( Params::WRAPPING_MODE_Z ) );
	m_filteringMode = QueryTypedValue< ValueIntPtr >( GetValue( Params::FILTERING_MODE ) );
	m_borderColor	= QueryTypedValue< ValueVec4Ptr >( GetValue( Params::BORDER_COLOR ) );

	Update();
}

} //model
} //bv
