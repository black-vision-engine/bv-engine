#pragma once

#include "CoreDEF.h"

namespace bv {

struct Glyph
{

    UInt16          code;       // unicode value
    UInt16          index;      // freetype glyph index

    UInt32          width;      // width of the glyph in pixels
    UInt32          height;     // height of the glyph in pixels
    Int32           advanceX;    // number of pixels to advance on x axis
    Int32           advanceY;    // number of pixels to advance on y axis

    Int32           bearingX;   // x offset of top-left corner from x axis
    Int32           bearingY;   // y offset of top-left corner from y axis

    UInt32          textureX;   // x pixel coord of the bitmap's bottom-left corner
    UInt32          textureY;   // y pixel coord of the bitmap's bottom-left corner

    SizeType        padding;

    SizeType        size;   // 

};

} // bv