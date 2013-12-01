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
    TextureWrappingMode m_wrappingMode;

    TextureInfo(    ResourceHandle * resHandle
               ,    const std::string & texName
               ,    const ParamTransform & texTransform = ParametersFactory::CreateParameter( "texTransform", TransformF() )
               ,    TextureWrappingMode wrappingMode = TextureWrappingMode::TWM_REPEAT );
};

} // model
} // bv