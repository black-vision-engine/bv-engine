#include "stdafx.h"

#include "SamplerShaderParameters.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// *******************************
//
			SamplerShaderParameters::SamplerShaderParameters			( SamplerWrappingMode wmX, SamplerWrappingMode wmY, SamplerWrappingMode wmZ, SamplerFilteringMode fm, const glm::vec4 & bc )
{
	m_wrappingModeX = new ShaderParamInt( "wrapModeX", ( int )wmX );
	m_wrappingModeY = new ShaderParamInt( "wrapModeY", ( int )wmY );
	m_wrappingModeZ = new ShaderParamInt( "wrapModeZ", ( int )wmZ );
	m_filteringMode = new ShaderParamInt( "filterMode", ( int )fm );
	m_borderColor	= new ShaderParamVec4( "borderColor", bc );
}

// *******************************
//
			SamplerShaderParameters::~SamplerShaderParameters			()
{
	delete m_wrappingModeX;
	delete m_wrappingModeY;
	delete m_wrappingModeZ;
	delete m_filteringMode;
	delete m_borderColor;
}

// *******************************
//
SamplerFilteringMode   SamplerShaderParameters::GetFilteringMode		() const
{
	return ( SamplerFilteringMode )m_filteringMode->GenericGetValue< int >();
}

// *******************************
//
SamplerWrappingMode		SamplerShaderParameters::GetWrappingModeX		() const
{
	return ( SamplerWrappingMode )m_wrappingModeX->GenericGetValue< int >();
}

// *******************************
//
SamplerWrappingMode		SamplerShaderParameters::GetWrappingModeY		() const
{
	return ( SamplerWrappingMode )m_wrappingModeY->GenericGetValue< int >();
}

// *******************************
//
SamplerWrappingMode		SamplerShaderParameters::GetWrappingModeZ		() const
{
	return ( SamplerWrappingMode )m_wrappingModeZ->GenericGetValue< int >();
}

// *******************************
//
const glm::vec4 &		SamplerShaderParameters::GetBorderColor			() const
{
	return m_borderColor->GenericGetValue< glm::vec4 >();
}

// *******************************
//
void					SamplerShaderParameters::SetFilteringMode		( SamplerFilteringMode val )
{
	m_filteringMode->SetValue( ( int )val );
}

// *******************************
//
void					SamplerShaderParameters::SetWrappingModeX		( SamplerWrappingMode val )
{
	m_wrappingModeX->SetValue( ( int )val );
}

// *******************************
//
void					SamplerShaderParameters::SetWrappingModeY		( SamplerWrappingMode val )
{
	m_wrappingModeY->SetValue( ( int )val );
}

// *******************************
//
void					SamplerShaderParameters::SetWrappingModeZ		( SamplerWrappingMode val )
{
	m_wrappingModeZ->SetValue( ( int )val );
}

// *******************************
//
void					SamplerShaderParameters::SetBorderColor			( const glm::vec4 & val )
{
	m_borderColor->SetValue( val );
}

} //bv