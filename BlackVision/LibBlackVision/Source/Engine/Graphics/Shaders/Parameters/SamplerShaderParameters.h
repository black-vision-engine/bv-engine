#pragma once

#include "Engine/Graphics/Shaders/Parameters/ShaderParamInt.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec4.h"
#include "Engine/Graphics/Shaders/TextureSampler.h"

namespace bv {

class SamplerShaderParameters
{
private:

	ShaderParamInt *	m_wrappingModeX;
	ShaderParamInt *	m_wrappingModeY;
	ShaderParamInt *	m_wrappingModeZ;
	ShaderParamInt *	m_filteringMode;
	ShaderParamVec4	*	m_borderColor;

public:
							SamplerShaderParameters		( SamplerWrappingMode wmX, SamplerWrappingMode wmY, SamplerWrappingMode wmZ, SamplerFilteringMode fm, const glm::vec4 & bc );
							~SamplerShaderParameters	();

    SamplerFilteringMode    GetFilteringMode			() const;
    SamplerWrappingMode		GetWrappingModeX			() const;
    SamplerWrappingMode		GetWrappingModeY			() const;
    SamplerWrappingMode		GetWrappingModeZ			() const;
    const glm::vec4 &		GetBorderColor				() const;

	void					SetFilteringMode			( SamplerFilteringMode );
    void					SetWrappingModeX			( SamplerWrappingMode );
    void					SetWrappingModeY			( SamplerWrappingMode );
    void					SetWrappingModeZ			( SamplerWrappingMode );
    void					SetBorderColor				( const glm::vec4 & );
};

DEFINE_PTR_TYPE(SamplerShaderParameters)
DEFINE_CONST_PTR_TYPE(SamplerShaderParameters)

} //bv
