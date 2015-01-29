#pragma once

#include "System/BasicTypes.h"

namespace bv { namespace model {

struct Glyph
{
  unsigned short    code;       // unicode value
  unsigned short    index;      // freetype glyph index

  SizeType		    width;      // width of the glyph in pixels
  SizeType		    height;     // height of the glyph in pixels
  Int32				advanceX;    // number of pixels to advance on x axis
  Int32				advanceY;    // number of pixels to advance on y axis

  int               bearingX;   // x offset of top-left corner from x axis
  int               bearingY;   // y offset of top-left corner from y axis

  SizeType			textureX;   // x pixel coord of the bitmap's bottom-left corner
  SizeType			textureY;   // y pixel coord of the bitmap's bottom-left corner

  SizeType			padding;

  SizeType			size;   // 

};

} // model
} // bv