#include "BVGLDefs.h"

namespace bv {

#define GL_MAP_WRITE_BIT						0x0002
#define GL_MAP_INVALIDATE_BUFFER_BIT			0x0008

#define GL_UNSIGNED_SHORT						0x1403
#define GL_UNSIGNED_INT							0x1405
#define GL_ELEMENT_ARRAY_BUFFER					0x8893

#define GL_STATIC_READ							0x88E5

#ifndef GL_ARB_draw_indirect
    #define GL_DRAW_INDIRECT_BUFFER				0x8F3F
    #define GL_DRAW_INDIRECT_BUFFER_BINDING		0x8F43
#endif

}