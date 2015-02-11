#pragma once

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexArray.h"

namespace bv {

class GeometryBuilder
{
public:

    static RenderableArrayDataArraysSingleVertexBuffer *    CreatreRectangle    ( float w, float h );

private:

    static void                                             SetUpRectTriStrip   ( float * buf, float w, float h );                
};

} //bv
