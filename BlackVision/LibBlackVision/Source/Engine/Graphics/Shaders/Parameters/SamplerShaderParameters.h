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
							SamplerShaderParameters		( ValueIntPtr wmX, ValueIntPtr wmY, ValueIntPtr wmZ, ValueIntPtr fm, ValueVec4Ptr bc );
							~SamplerShaderParameters	();

    SamplerFilteringMode    GetFilteringMode			() const;
    SamplerWrappingMode		GetWrappingModeX			() const;
    SamplerWrappingMode		GetWrappingModeY			() const;
    SamplerWrappingMode		GetWrappingModeZ			() const;
    const glm::vec4 &		GetBorderColor				() const;
};

DEFINE_PTR_TYPE(SamplerShaderParameters)
DEFINE_CONST_PTR_TYPE(SamplerShaderParameters)

} //bv
