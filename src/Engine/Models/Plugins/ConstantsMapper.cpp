#include "ConstantsMapper.h"

namespace bv { namespace model {

SamplerWrappingMode              ConstantsMapper::EngineConstant( TextureWrappingMode textureMappingMode )
{
    return SamplerWrappingMode( (int)textureMappingMode );
}

SamplerFilteringMode             ConstantsMapper::EngineConstant( TextureFilteringMode textureFilteringMode )
{
    return SamplerFilteringMode( (int)textureFilteringMode );
}


} // model
} // bv