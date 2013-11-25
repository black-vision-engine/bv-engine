#pragma once

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>

#include "Engine/Graphics/Resources/DataBuffer.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Shaders/TextureSampler.h"
#include "Engine/Graphics/State/StateEnums.h"


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
};

}
