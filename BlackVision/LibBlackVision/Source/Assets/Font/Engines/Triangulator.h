#pragma once

/*
* FTGL - OpenGL font library
*
* Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
* Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "FTGL/ftgl.h"

#include "FTContour.h"
#include "FTList.h"
#include "FTVector.h"
#include "FTVectoriser.h"


#ifndef CALLBACK
#define CALLBACK
#endif


/**
* FTVectoriser class is a helper class that converts font outlines into
* point data.
*
* Implementation based on ftgl Vectorizer class.
*/
class Triangulator
{
public:
    /**
    * Constructor
    *
    * @param glyph The freetype glyph to be processed
    */
    Triangulator( const FT_GlyphSlot glyph );

    /**
    *  Destructor
    */
    virtual ~Triangulator();

    /**
    * Build an FTMesh from the vector outline data.
    *
    * @param zNormal   The direction of the z axis of the normal
    *                  for this mesh
    * FIXME: change the following for a constant
    * @param outsetType Specify the outset type contour
    *  0 : Original
    *  1 : Front
    *  2 : Back
    * @param outsetSize Specify the outset size contour
    */
    void MakeMesh( FTGL_DOUBLE zNormal = FTGL_FRONT_FACING, int outsetType = 0, float outsetSize = 0.0f );

    /**
    * Get the current mesh.
    */
    const FTMesh* const GetMesh() const { return mesh; }

    /**
    * Get the total count of points in this outline
    *
    * @return the number of points
    */
    size_t PointCount();

    /**
    * Get the count of contours in this outline
    *
    * @return the number of contours
    */
    size_t ContourCount() const { return ftContourCount; }

    /**
    * Return a contour at index
    *
    * @return the number of contours
    */
    const FTContour* const Contour( size_t index ) const;

    /**
    * Get the number of points in a specific contour in this outline
    *
    * @param c     The contour index
    * @return      the number of points in contour[c]
    */
    size_t ContourSize( int c ) const { return contourList[ c ]->PointCount(); }

    /**
    * Get the flag for the tesselation rule for this outline
    *
    * @return The contour flag
    */
    int ContourFlag() const { return contourFlag; }

private:
    /**
    * Process the freetype outline data into contours of points
    *
    * @param front front outset distance
    * @param back back outset distance
    */
    void ProcessContours();

    /**
    * The list of contours in the glyph
    */
    FTContour** contourList;

    std::vector< int >                  contoursNesting;
    std::vector< std::vector< bool > >  contoursIncuding;

    /**
    * A Mesh for tesselations
    */
    FTMesh* mesh;

    /**
    * The number of contours reported by Freetype
    */
    short ftContourCount;

    /**
    * A flag indicating the tesselation rule for the glyph
    */
    int contourFlag;

    /**
    * A Freetype outline
    */
    FT_Outline outline;
};



