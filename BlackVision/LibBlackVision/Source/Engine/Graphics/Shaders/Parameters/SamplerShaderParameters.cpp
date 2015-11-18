#include "SamplerShaderParameters.h"


namespace bv {

// *******************************
//
			SamplerShaderParameters::SamplerShaderParameters			( ValueIntPtr wmX, ValueIntPtr wmY, ValueIntPtr wmZ, ValueIntPtr fm, ValueVec4Ptr bc )
{
	m_wrappingModeX = new ShaderParamInt( wmX->GetName(), wmX.get() );
	m_wrappingModeY = new ShaderParamInt( wmY->GetName(), wmY.get() );
	m_wrappingModeZ = new ShaderParamInt( wmZ->GetName(), wmZ.get() );
	m_filteringMode = new ShaderParamInt( fm->GetName(), fm.get() );
	m_borderColor	= new ShaderParamVec4( bc->GetName(), bc.get() );
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

} //bv