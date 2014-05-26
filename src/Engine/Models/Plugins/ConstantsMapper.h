#pragma once

#include "Engine/Graphics/Shaders/TextureSampler.h"
#include "Engine/Types/Enums.h"


namespace bv {

struct ConstantsMapper
{
    static SamplerWrappingMode              EngineConstant( TextureWrappingMode textureMappingMode );
    static SamplerFilteringMode             EngineConstant( TextureFilteringMode textureFilteringMode );
};

} // bv
