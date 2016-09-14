/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Éric Beets <ericbeets@free.fr>
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

#include "config.h"

//#include "FTInternals.h"
#include "FTVectoriser.h"

#include "poly2tri.h"

#ifndef CALLBACK
#define CALLBACK
#endif

typedef float GLfloat;
#define GL_TRIANGLES                      0x0004

//#if defined __APPLE_CC__ && __APPLE_CC__ < 5465
//    typedef GLvoid (*GLUTesselatorFunction) (...);
//#elif defined WIN32 && !defined __CYGWIN__
//    typedef GLvoid (CALLBACK *GLUTesselatorFunction) ();
//#else
//    typedef GLvoid (*GLUTesselatorFunction) ();
//#endif


void CALLBACK ftglError(GLenum errCode, FTMesh* mesh)
{
    mesh->Error(errCode);
}


void CALLBACK ftglVertex(void* data, FTMesh* mesh)
{
    FTGL_DOUBLE* vertex = static_cast<FTGL_DOUBLE*>(data);
    mesh->AddPoint(vertex[0], vertex[1], vertex[2]);
}


void CALLBACK ftglCombine(FTGL_DOUBLE coords[3], void* vertex_data[4], GLfloat weight[4], void** outData, FTMesh* mesh)
{
    const FTGL_DOUBLE* vertex = static_cast<const FTGL_DOUBLE*>(coords);
    *outData = const_cast<FTGL_DOUBLE*>(mesh->Combine(vertex[0], vertex[1], vertex[2]));
}

void CALLBACK ftglBegin(GLenum type, FTMesh* mesh)
{
    mesh->Begin(type);
}


void CALLBACK ftglEnd(FTMesh* mesh)
{
    mesh->End();
}


FTMesh::FTMesh()
: currentTesselation(0),
    err(0)
{
    tesselationList.reserve(16);
}


FTMesh::~FTMesh()
{
    for(size_t t = 0; t < tesselationList.size(); ++t)
    {
        delete tesselationList[t];
    }

    tesselationList.clear();
}


void FTMesh::AddPoint(const FTGL_DOUBLE x, const FTGL_DOUBLE y, const FTGL_DOUBLE z)
{
    currentTesselation->AddPoint(x, y, z);
}


const FTGL_DOUBLE* FTMesh::Combine(const FTGL_DOUBLE x, const FTGL_DOUBLE y, const FTGL_DOUBLE z)
{
    tempPointList.push_back(FTPoint(x, y,z));
    return static_cast<const FTGL_DOUBLE*>(tempPointList.back());
}


void FTMesh::Begin(GLenum meshType)
{
    currentTesselation = new FTTesselation(meshType);
}


void FTMesh::End()
{
    tesselationList.push_back(currentTesselation);
}


const FTTesselation* const FTMesh::Tesselation(size_t index) const
{
    return (index < tesselationList.size()) ? tesselationList[index] : NULL;
}


FTVectoriser::FTVectoriser(const FT_GlyphSlot glyph)
:   contourList(0),
    mesh(0),
    ftContourCount(0),
    contourFlag(0)
{
    if(glyph)
    {
        outline = glyph->outline;

        ftContourCount = outline.n_contours;
        contourList = 0;
        contourFlag = outline.flags;

        ProcessContours();
    }
}


FTVectoriser::~FTVectoriser()
{
    for(size_t c = 0; c < ContourCount(); ++c)
    {
        delete contourList[c];
    }

    delete [] contourList;
    delete mesh;
}


void FTVectoriser::ProcessContours()
{
    short contourLength = 0;
    short startIndex = 0;
    short endIndex = 0;

    contourList = new FTContour*[ftContourCount];

    for(int i = 0; i < ftContourCount; ++i)
    {
        FT_Vector* pointList = &outline.points[startIndex];
        char* tagList = &outline.tags[startIndex];

        endIndex = outline.contours[i];
        contourLength =  (endIndex - startIndex) + 1;

        FTContour* contour = new FTContour(pointList, tagList, contourLength);

        contourList[i] = contour;

        startIndex = endIndex + 1;
    }

    auto orient = FT_Outline_Get_Orientation( &outline );

    // Compute each contour's parity. FIXME: see if FT_Outline_Get_Orientation
    // can do it for us.
    for(int i = 0; i < ftContourCount; i++)
    {
        FTContour *c1 = contourList[i];

        // Make sure the glyph has the proper orientation.
        if( orient == FT_ORIENTATION_POSTSCRIPT )
            c1->SetParity( true );
    }
}


size_t FTVectoriser::PointCount()
{
    size_t s = 0;
    for(size_t c = 0; c < ContourCount(); ++c)
    {
        s += contourList[c]->PointCount();
    }

    return s;
}


const FTContour* const FTVectoriser::Contour(size_t index) const
{
    return (index < ContourCount()) ? contourList[index] : NULL;
}


void FTVectoriser::MakeMesh(FTGL_DOUBLE zNormal, int outsetType, float outsetSize)
{
    if(mesh)
    {
        delete mesh;
    }

    mesh = new FTMesh;

    std::vector< const FTContour* > outerContours;      // Contours which must be filled. Other contours will be treated as holes.
    std::vector< int > ocIdx;                           // Indicies of outer contours.
    std::vector< std::vector< p2t::Point * > > contoursVecPointsVec;
    
    outerContours.reserve( ftContourCount );
    ocIdx.reserve( ftContourCount );
    contoursVecPointsVec.resize( ftContourCount );


    for( size_t c = 0; c < ContourCount(); ++c )
    {
        const FTContour* contour = contourList[ c ];

        if( contour->IsOuterContour() )
        {
            outerContours.push_back( contour );
            ocIdx.push_back( c );
        }

        switch( outsetType )
        {
            case 1: contourList[ c ]->buildFrontOutset( outsetSize ); break;
            case 2: contourList[ c ]->buildBackOutset( outsetSize ); break;
        }

        std::vector< p2t::Point * >& polyline = contoursVecPointsVec[ c ];
        polyline.reserve( contour->PointCount() );

        for( size_t p = 0; p < contour->PointCount(); ++p )
        {
            p2t::Point * d = nullptr;
            switch( outsetType )
            {
                case 1: d = new p2t::Point( contour->FrontPoint( p ).X(), contour->FrontPoint( p ).Y() );
                    break;
                case 2: d = new p2t::Point( contour->BackPoint( p ).X(), contour->BackPoint( p ).Y() );
                    break;
                case 0:
                default:
                    d = new p2t::Point( contour->Point( p ).X(), contour->Point( p ).Y() );
                    break;
            }

            polyline.push_back( d );

        }
    }

    // Process only outer contours. Check intersections with rest countours and add as holes
    // only this ones, that are inside bounding box. 
    for( int i = 0; i < outerContours.size(); ++i )
    {
        p2t::CDT * cdt = new p2t::CDT( contoursVecPointsVec[ ocIdx[ i ] ] );

        for( int c = ocIdx[ i ] + 1; c < ContourCount(); ++c )
        {
            const FTContour* contour = contourList[ c ];
            if( contour->IsOuterContour() )
                break;

            if( outerContours[ i ]->Intersects( contour ) )
            {
                cdt->AddHole( contoursVecPointsVec[ c ] );
            }
        }

        cdt->Triangulate();

        mesh->Begin( GL_TRIANGLES );

        for( auto t : cdt->GetTriangles() )
        {
            auto p0 = t->GetPoint( 0 );
            auto p1 = t->GetPoint( 1 );
            auto p2 = t->GetPoint( 2 );
            mesh->AddPoint( p0->x, p0->y, 0.0 );
            mesh->AddPoint( p1->x, p1->y, 0.0 );
            mesh->AddPoint( p2->x, p2->y, 0.0 );
        }

        mesh->End();

        delete cdt;
    }


}

