#pragma once

#include "Engine/Graphics/Renderers/OGLRenderer/GL/BVGL.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"


namespace bv {

enum class MemoryLockingType : int
{
    MLT_READ_ONLY = 0,
    MLT_WRITE_ONLY,
    MLT_READ_WRITE,

    MLT_TOTAL
};

class ConstantsMapper
{
public:

    static GLuint GLConstant( MemoryLockingType mlt );
    static GLuint GLConstant( DataBuffer::Semantic semantic );
    static GLuint GLConstant( RenderableEntity::RenderableType type );
    static GLuint GLConstant( SamplerWrappingMode wrappingMode );
    static GLuint GLConstant( SamplerFilteringMode filteringMode );
    static GLuint GLConstant( AlphaSrcBlendMode mode );
    static GLuint GLConstant( AlphaDstBlendMode mode );
    static GLuint GLConstant( DepthCompareMode mode );
    static GLuint GLConstant( FillStateMode mode );

    static GLuint GLConstantTextureFormat           ( TextureFormat fmt );
    static GLuint GLConstantTextureInternalFormat   ( TextureFormat fmt );
    static GLuint GLConstantTextureType             ( TextureFormat fmt );

};

}
