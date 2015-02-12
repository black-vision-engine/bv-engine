#pragma once

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexArray.h"

namespace bv {

class GeometryBuilder
{
public:

    static RenderableArrayDataArraysSingleVertexBuffer *    CreatreRectangle    ( float w, float h, float z, bool withUV );

private:

    static void                                             SetUpRectTriStrip   ( char * bufChar, float w, float h, float z, bool addUV );                
};

} //bv
