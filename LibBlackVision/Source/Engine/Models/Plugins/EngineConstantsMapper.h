#pragma once

#include "Engine/Graphics/Shaders/TextureSampler.h"
#include "Engine/Types/Enums.h"


namespace bv {

struct EngineConstantsMapper
{
    static SamplerWrappingMode              EngineConstant( TextureWrappingMode textureMappingMode );
    static SamplerFilteringMode             EngineConstant( TextureFilteringMode textureFilteringMode );
};

} // bv
