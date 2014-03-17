#include "ConstantsMapper.h"

namespace bv {

SamplerWrappingMode              ConstantsMapper::EngineConstant( TextureWrappingMode textureMappingMode )
{
    return SamplerWrappingMode( (int)textureMappingMode );
}

SamplerFilteringMode             ConstantsMapper::EngineConstant( TextureFilteringMode textureFilteringMode )
{
    return SamplerFilteringMode( (int)textureFilteringMode );
}

} // bv
