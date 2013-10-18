#pragma once

namespace bv {

struct Glyph
{
  unsigned short    code;       // unicode value
  unsigned short    index;      // freetype glyph index

  unsigned int      width;      // width of the glyph in pixels
  unsigned int      height;     // height of the glyph in pixels
  unsigned int      advance;    // number of pixels to advance on x axis

  int               bearingX;   // x offset of top-left corner from x axis
  int               bearingY;   // y offset of top-left corner from y axis

  unsigned int      textureX;   // x pixel coord of the bitmap's bottom-left corner
  unsigned int      textureY;   // y pixel coord of the bitmap's bottom-left corner

  unsigned int      padding;

  unsigned int      size;   // 
};

}
