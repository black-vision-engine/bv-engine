#pragma once

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Types/Values/ValuesFactory.h"

#include <string>


namespace bv { namespace model {

class ResourceHandle;

struct TextureInfo
{
    const ResourceHandle *  m_resHandle;
    std::string             m_texName;

    ParamTransform          m_texTransform;
    ValueMat4Ptr            m_texTransformVal;

    ParamFloat              m_texAlpha;
    ValueFloatPtr           m_texAlphaVal;

    ParamVec4               m_texBorderColor;
    ValueVec4Ptr            m_texBorderColorVal;

    TextureWrappingMode     m_wrappingModeX;
    TextureWrappingMode     m_wrappingModeY;
    TextureFilteringMode    m_filteringMode;

    TextureInfo(    const ResourceHandle * resHandle
               ,    const std::string & texName
               ,    const ParamTransform & texTransform = ParametersFactory::CreateParameter( "texTransform", TransformF() )
               ,    const ParamFloat& alpha = ParametersFactory::CreateParameter( "texAlpha", InterpolatorsHelper::CreateConstValue( 1.f ) )
               ,    const ParamVec4& borderColor = ParametersFactory::CreateParameter( "texBorderColor", InterpolatorsHelper::CreateConstValue( glm::vec4( 0.f, 0.f, 1.f, 0.f ) ) )
               ,    TextureWrappingMode wrappingModeX = TextureWrappingMode::TWM_REPEAT 
               ,    TextureWrappingMode wrappingModeY = TextureWrappingMode::TWM_REPEAT
               ,    TextureFilteringMode filteringMode = TextureFilteringMode::TFM_LINEAR
               );
};

} // model
} // bv