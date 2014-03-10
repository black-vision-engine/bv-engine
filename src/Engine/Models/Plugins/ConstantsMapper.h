#pragma once

#include "PluginEnums.h"

#include "Engine/Graphics/Shaders/TextureSampler.h"

namespace bv { namespace model {

struct ConstantsMapper
{
    static SamplerWrappingMode              EngineConstant( TextureWrappingMode textureMappingMode );
    static SamplerFilteringMode             EngineConstant( TextureFilteringMode textureFilteringMode );
};

} // model
} // bv
