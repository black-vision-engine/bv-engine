#pragma once

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>

#include "DataBuffer.h"
#include "RenderableEntity.h"

#include "Engine\Graphics\Shaders\TextureSampler.h"

namespace bv {

enum class MemoryLockingType : int
{
    MLT_READ_ONLY = 0,
    MLT_WRITE_ONLY,
    MLT_READ_WRITE,

    MLT_TOTAL
};

enum class FaceKind : int
{
    FK_FRONT = 0,
    FK_BACK,
    FK_FRONT_AND_BACK,
};

class ConstantsMapper
{
public:

    static GLuint GlConstant( MemoryLockingType mlt );
    static GLuint GlConstant( DataBuffer::Semantic semantic );
    static GLuint GlConstant( RenderableEntity::RenderableType type );
    static GLuint GlConstant( FaceKind face );
    static GLuint GlConstant( SamplerWrappingMode wrappingMode );
    static GLuint GlConstant( SamplerFilteringMode filteringMode );

};


}