#include "stdafx.h"

#include "EngineConstantsMapper.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

SamplerWrappingMode              EngineConstantsMapper::EngineConstant( TextureWrappingMode textureMappingMode )
{
    return SamplerWrappingMode( (int)textureMappingMode );
}

SamplerFilteringMode             EngineConstantsMapper::EngineConstant( TextureFilteringMode textureFilteringMode )
{
    return SamplerFilteringMode( (int)textureFilteringMode );
}

} // bv
