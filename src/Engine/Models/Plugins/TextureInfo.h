#pragma once

#include "Engine/Models/Plugins/PluginEnums.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"
#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include <string>

namespace bv { namespace model {

class ResourceHandle;

struct TextureInfo
{
    ResourceHandle *    m_resHandle;
    std::string         m_texName;
    ParamTransform      m_texTransform;
    ValueMat4*          m_texTransformVal;
    TextureWrappingMode m_wrappingModeX;
    TextureWrappingMode m_wrappingModeY;

    TextureInfo(    ResourceHandle * resHandle
               ,    const std::string & texName
               ,    const ParamTransform & texTransform = ParametersFactory::CreateParameter( "texTransform", TransformF() )
               ,    TextureWrappingMode wrappingModeX = TextureWrappingMode::TWM_REPEAT 
               ,    TextureWrappingMode wrappingModeY = TextureWrappingMode::TWM_REPEAT
               );
};

} // model
} // bv