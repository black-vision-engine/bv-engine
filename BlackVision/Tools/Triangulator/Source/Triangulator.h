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


//#include "ftgl.h"

/* We need the Freetype headers */
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

/* Floating point types used by the library */
typedef double   FTGL_DOUBLE;
typedef float    FTGL_FLOAT;


#include "FTContour.h"
#include "FTList.h"
#include "FTVector.h"

#include "Mesh.h"
#include "PolylineValidator.h"

#include <vector>
#include <memory>

namespace ClipperLib
{
    class PolyTree;
    class PolyNode;
}


/**
* FTVectoriser class is a helper class that converts font outlines into
* point data.
*
* Implementation based on ftgl Vectorizer class.
*/
class Triangulator
{
public:
    enum FillRule
    {
        EvenOdd,
        NonZero,
        Positive,
        Negative
    };
public:

    Triangulator( ContoursList && contours );
	Triangulator( ContoursList && contours, const std::string & debugFileName );
    Triangulator( ContoursList && contours, const std::string & debugFileName, const std::string & contourName );

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
    Mesh MakeMesh();


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
    size_t ContourCount() const { return m_contoursList.size(); }

    /**
    * Get the number of points in a specific contour in this outline
    *
    * @param c     The contour index
    * @return      the number of points in contour[c]
    */
    size_t ContourSize( int c ) const { return m_contoursList[ c ]->PointCount(); }

	const std::vector< int > &						GetNestingArray		()		{ return m_contoursNesting; }
	const std::vector< std::vector< bool > > &		GetIncludingArray	()		{ return m_contoursIncuding; }
    const std::vector< IntersectionsVec > &         GetSelfIntersections()      { return m_selfIntersections;  }
    const PolylinesVec &                            GetPolylines        ()      { return m_polylines;  }
    const ContoursList &                            GetContours         ()      { return m_contoursList;  }

    void                                            PrintContoursToFile ();
    void                                            PrintToFileAsUnitTest();

    Polyline &&                                     HeuristicFindMainContour    ( PolylinesVec && polylines );

    void                                            SetFillRule         ( FillRule rule );

private:

    FillRule                                m_fillRule;

	ContoursList							m_contoursList;
    PolylinesVec                            m_polylines;

    std::vector< int >						m_contoursNesting;
    std::vector< std::vector< bool > >		m_contoursIncuding;
    std::vector< IntersectionsVec >         m_selfIntersections;

	bool									m_printContoursToFile;
	std::string								m_fileName;
    std::string                             m_contourName;



private:
    /**
    * Process the freetype outline data into contours of points
    *
    * @param front front outset distance
    * @param back back outset distance
    */
    void        ProcessContours();

    void        TriangulateHierarchy    ( ClipperLib::PolyNode & treeNode, Mesh & mesh, uint64_t rescale );
};



