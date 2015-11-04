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
	AddEvaluators( timeEvaluator );

    //Set default values of all parameters
	SetWrappingModeX( TextureWrappingMode::TWM_CLAMP_BORDER );
	SetWrappingModeY( TextureWrappingMode::TWM_CLAMP_BORDER );
	SetWrappingModeZ( TextureWrappingMode::TWM_CLAMP_BORDER );
	SetFilteringMode( TextureFilteringMode::TFM_LINEAR );
	SetBorderColor( glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
}

// **************************
//
SamplerStateModel::SamplerStateModel	( ITimeEvaluatorPtr timeEvaluator, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureWrappingMode wmz, TextureFilteringMode fm, const glm::vec4 & bc )
{
	AddEvaluators( timeEvaluator );

	SetWrappingModeX( wmx );
	SetWrappingModeY( wmy );
	SetWrappingModeZ( wmz );
	SetFilteringMode( fm );
	SetBorderColor( bc );
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
ValueIntPtr				SamplerStateModel::GetWrappingModeX		() const
{
	return m_wrappingModeY;
}

// **************************
//
ValueIntPtr				SamplerStateModel::GetWrappingModeY		() const
{
	return m_wrappingModeY;
}

// **************************
//
ValueIntPtr				SamplerStateModel::GetWrappingModeZ		() const
{
	return m_wrappingModeZ;
}

// **************************
//
ValueIntPtr				SamplerStateModel::GetFilteringMode		() const
{
	return m_filteringMode;
}

// **************************
//
ValueVec4Ptr			SamplerStateModel::GetBorderColor		() const
{
	return m_borderColor;
}

// **************************
//
void					SamplerStateModel::SetWrappingModeX		( TextureWrappingMode wm )
{
	return  m_wrappingModeX->SetValue( static_cast< int >( wm ) );
}

// **************************
//
void					SamplerStateModel::SetWrappingModeY		( TextureWrappingMode wm )
{
	return  m_wrappingModeY->SetValue( static_cast< int >( wm ) );
}

// **************************
//
void					SamplerStateModel::SetWrappingModeZ		( TextureWrappingMode wm )
{
	return  m_wrappingModeZ->SetValue( static_cast< int >( wm ) );
}

// **************************
//
void					SamplerStateModel::SetFilteringMode		( TextureFilteringMode fm )
{
	return  m_filteringMode->SetValue( static_cast< int >( fm ) );
}

// **************************
//
void					SamplerStateModel::SetBorderColor		( const glm::vec4 & bc )
{
	return  m_borderColor->SetValue( bc );
}

// **************************
//
void					SamplerStateModel::AddEvaluators		( ITimeEvaluatorPtr timeEvaluator )
{
	RegisterAll( ParamValEvaluatorFactory::CreateSimpleIntEvaluator( Params::WRAPPING_MODE_X, timeEvaluator ) );
	RegisterAll( ParamValEvaluatorFactory::CreateSimpleIntEvaluator( Params::WRAPPING_MODE_Y, timeEvaluator ) );
	RegisterAll( ParamValEvaluatorFactory::CreateSimpleIntEvaluator( Params::WRAPPING_MODE_Z, timeEvaluator ) );
	RegisterAll( ParamValEvaluatorFactory::CreateSimpleIntEvaluator( Params::FILTERING_MODE, timeEvaluator ) );
	RegisterAll( ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( Params::BORDER_COLOR, timeEvaluator ) );

	m_wrappingModeX = QueryTypedValue< ValueIntPtr >( GetValue( Params::WRAPPING_MODE_X ) );
	m_wrappingModeY = QueryTypedValue< ValueIntPtr >( GetValue( Params::WRAPPING_MODE_Y ) );
	m_wrappingModeZ = QueryTypedValue< ValueIntPtr >( GetValue( Params::WRAPPING_MODE_Z ) );
	m_filteringMode = QueryTypedValue< ValueIntPtr >( GetValue( Params::FILTERING_MODE ) );
	m_borderColor = QueryTypedValue< ValueVec4Ptr >( GetValue( Params::BORDER_COLOR ) );
}

} //model
} //bv
