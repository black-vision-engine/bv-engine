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

//#include "config.h"

//#include "FTInternals.h"
#include "Triangulator.h"
#include "PolylineValidator.h"

#include "Clipper/clipper.hpp"
#include "poly2tri/poly2tri.h"


// Debug
#include <fstream>



Polyline                    AllocatePolyline    ( const ClipperLib::Path & path );
ClipperLib::PolyFillType    GetFillRule         ( Triangulator::FillRule rule );


// ***********************
//
// http://stackoverflow.com/questions/364985/algorithm-for-finding-the-smallest-power-of-two-thats-greater-or-equal-to-a-giv
inline int64_t  RoundUpPowerOf2 ( int64_t x )
{
    if( x < 0 )
        return 0;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    return x + 1;
}


const float epsilon = 0.00001f;


// ================================ //
//
Triangulator::Triangulator( ContoursList && contours )
	:	m_contoursList( std::move( contours ) )
	,	m_printContoursToFile( false )
    ,   m_fillRule( FillRule::NonZero )
{
	ProcessContours();
}

// ***********************
//
Triangulator::Triangulator( ContoursList && contours, const std::string & debugFileName )
	:	m_contoursList( std::move( contours ) )
	,	m_printContoursToFile( true )
	,	m_fileName( debugFileName )
    ,   m_fillRule( FillRule::NonZero )
{
	ProcessContours();
}

// ***********************
//
Triangulator::Triangulator( ContoursList && contours, const std::string & debugFileName, const std::string & contourName )
    :   m_contoursList( std::move( contours ) )
    ,   m_printContoursToFile( true )
    ,   m_fileName( debugFileName )
    ,   m_contourName( contourName )
    ,   m_fillRule( FillRule::NonZero )
{
    ProcessContours();
}

// ================================ //
//
Triangulator::~Triangulator()
{
    for( auto & polyline : m_polylines )
    {
        for( auto point : polyline )
        {
            delete point;
        }
    }
}


// ================================ //
//
void Triangulator::ProcessContours()
{
    short contourLength = 0;
    short startIndex = 0;
    short endIndex = 0;

	auto ftContourCount = m_contoursList.size();

	m_contoursList.resize( ftContourCount );
}


// ================================ //
//
size_t Triangulator::PointCount()
{
    size_t s = 0;
    for(size_t c = 0; c < ContourCount(); ++c)
    {
        s += m_contoursList[c]->PointCount();
    }

    return s;
}

// ================================ //
//
Mesh Triangulator::MakeMesh()
{
	Mesh mesh( true );
	auto ftContourCount = m_contoursList.size();

    m_polylines.reserve( ftContourCount );

    ClipperLib::Paths polylinesPaths;
    polylinesPaths.resize( ftContourCount );


    // Compute bounding box of our contours.
    double minX = HUGE;
    double minY = HUGE;
    double maxX = -HUGE;
    double maxY = -HUGE;

    for( size_t c = 0; c < ContourCount(); ++c )
    {
        const auto & contour = m_contoursList[ c ];

        if( contour->GetMinX() < minX )    minX = contour->GetMinX();
        if( contour->GetMinY() < minY )    minY = contour->GetMinY();
        if( contour->GetMaxX() > maxX )    maxX = contour->GetMaxX();
        if( contour->GetMaxY() > maxY )    maxY = contour->GetMaxY();
    }

    int64_t scaleFloat = static_cast< uint64_t >( std::numeric_limits< int >::max() / std::max( maxX - minX, maxY - minY ) );
    scaleFloat = RoundUpPowerOf2( scaleFloat );

    minX = minX * scaleFloat;
    minY = minY * scaleFloat;
    maxX = maxX * scaleFloat;
    maxY = maxY * scaleFloat;


    for( size_t c = 0; c < ContourCount(); ++c )
    {
        const auto & contour = m_contoursList[ c ];
        polylinesPaths[ c ].reserve( contour->PointCount() );

        for( size_t p = 0; p < contour->PointCount(); ++p )
        {
            polylinesPaths[ c ] << ClipperLib::IntPoint( static_cast< ClipperLib::cInt >( contour->Point( p ).X() * scaleFloat ), static_cast< ClipperLib::cInt >( contour->Point( p ).Y() * scaleFloat ) );
        }
    }

    // Bounding box of our contours as Path.
    ClipperLib::Path subPath;
    double offset = 100.0;
    subPath << ClipperLib::IntPoint( static_cast< ClipperLib::cInt >( minX - offset ), static_cast< ClipperLib::cInt >( minY - offset ) );
    subPath << ClipperLib::IntPoint( static_cast< ClipperLib::cInt >( minX - offset ), static_cast< ClipperLib::cInt >( maxY + offset ) );
    subPath << ClipperLib::IntPoint( static_cast< ClipperLib::cInt >( maxX + offset ), static_cast< ClipperLib::cInt >( maxY + offset ) );
    subPath << ClipperLib::IntPoint( static_cast< ClipperLib::cInt >( maxX + offset ), static_cast< ClipperLib::cInt >( minY - offset ) );

    ClipperLib::Clipper clipper;
    clipper.StrictlySimple( true );
    clipper.AddPaths( polylinesPaths, ClipperLib::PolyType::ptSubject, true );
    clipper.AddPath( subPath, ClipperLib::PolyType::ptClip, true );

    ClipperLib::PolyTree resultTree;
    clipper.Execute( ClipperLib::ClipType::ctIntersection, resultTree, GetFillRule( m_fillRule ) );


    for( auto & outerContour : resultTree.Childs )
    {
        TriangulateHierarchy( *outerContour, mesh, scaleFloat );
    }


    // Print contours to file for debug purposes.
    if( m_printContoursToFile )
    {
        PrintContoursToFile();
    }

	return mesh;
}


// ***********************
//
void    Triangulator::TriangulateHierarchy( ClipperLib::PolyNode & treeNode, Mesh & mesh, uint64_t rescale )
{
    // Prepare vector of polygons for cleaning.
    ClipperLib::Paths paths;
    auto childCount = treeNode.ChildCount();
    paths.reserve( childCount + 1 );

    if( !treeNode.Contour.empty() )
    {
        paths.push_back( std::move( treeNode.Contour ) );

        for( int j = 0; j < childCount; ++j )
        {
            if( !treeNode.Childs[ j ]->Contour.empty() )
            {
                paths.push_back( std::move( treeNode.Childs[ j ]->Contour ) );
            }
        }

        // Clean polygons (self intersection artifacts, colinears, duplicates)
        double distance = 1.5;//rescale * epsilon;
        ClipperLib::SimplifyPolygons( paths, GetFillRule( m_fillRule ) );
        ClipperLib::CleanPolygons( paths, distance );

        if( !paths.empty() && !paths[ 0 ].empty() )
        {
            // Triangulate current tree level withs holes level.
            // First copy outer path.
            m_polylines.push_back( AllocatePolyline( paths[ 0 ] ) );
            p2t::CDT * cdt = new p2t::CDT( m_polylines[ m_polylines.size() - 1 ] );

            // Add holes to triangulator.
            for( int j = 0; j < paths.size() - 1; ++j )
            {
                if( !paths[ j + 1 ].empty() )
                {
                    ClipperLib::Paths newPaths;

                    ClipperLib::ClipperOffset offseter;
                    offseter.AddPath( paths[ j + 1 ], ClipperLib::JoinType::jtSquare, ClipperLib::EndType::etClosedPolygon );
                    offseter.Execute( newPaths, -distance );
                    //ClipperLib::CleanPolygons( newPaths, distance );

                    for( int i = 0; i < newPaths.size(); ++i )
                    {
                        m_polylines.push_back( AllocatePolyline( newPaths[ i ] ) );
                        cdt->AddHole( m_polylines[ m_polylines.size() - 1 ] );
                    }
                }
            }

            cdt->Triangulate();

            // Copy verticies to mesh structure.
            mesh.Begin();

            for( auto t : cdt->GetTriangles() )
            {
                auto p0 = t->GetPoint( 0 );
                auto p1 = t->GetPoint( 1 );
                auto p2 = t->GetPoint( 2 );
                mesh.AddPoint( static_cast<float>( p0->x / rescale ), static_cast<float>( p0->y / rescale ), 0.0 );
                mesh.AddPoint( static_cast<float>( p1->x / rescale ), static_cast<float>( p1->y / rescale ), 0.0 );
                mesh.AddPoint( static_cast<float>( p2->x / rescale ), static_cast<float>( p2->y / rescale ), 0.0 );
            }

            mesh.End();

            delete cdt;
        }
    }

    // Process lower levels of nested paths.
    for( int j = 0; j < treeNode.ChildCount(); ++j )
    {
        auto & child = treeNode.Childs[ j ];
        for( int i = 0; i < child->ChildCount(); ++i )
        {
            TriangulateHierarchy( *child->Childs[ i ], mesh, rescale );
        }
    }
}

// ***********************
//
Polyline    AllocatePolyline    ( const ClipperLib::Path & path )
{
    Polyline poly;
    poly.reserve( path.size() );

    for( auto & point : path )
    {
        poly.push_back( new p2t::Point( (double)point.X, (double)point.Y ) );
    }

    return poly;
}


// ================================ //
//
void Triangulator::PrintContoursToFile()
{
    std::fstream file( m_fileName, std::ios_base::app );
    assert( !file.fail() );

// ***********************
// Header - beginning of shape
    file << std::endl << std::endl << "Next shape ";
    if( !m_contourName.empty() )
        file << m_contourName.c_str();
    file << std::endl;

    // ***********************
    // Contours sizes
    file << "Contours sizes: " << std::endl;
    for( int i = 0; i < m_polylines.size(); ++i )
    {
        file << m_polylines[ i ].size() << ", ";
    }
    file << std::endl;


// ***********************
// Polylines
    for( size_t c = 0; c < m_polylines.size(); c++ )
    {
        file << std::endl << "Contour number " << c << std::endl;

        // ***********************
        // Points
        auto & contour = m_polylines[ c ];
        for( size_t i = 0; i < m_polylines[ c ].size(); i++ )
        {
            file << contour[ i ]->x << " " << contour[ i ]->y << std::endl;
        }
    }

    file.close();
}


// ***********************
//
void                Triangulator::SetFillRule( FillRule rule )
{
    m_fillRule = rule;
}

// ***********************
//
ClipperLib::PolyFillType    GetFillRule ( Triangulator::FillRule rule )
{
    if( rule == Triangulator::FillRule::EvenOdd )
        return ClipperLib::PolyFillType::pftEvenOdd;
    else if( rule == Triangulator::FillRule::Negative )
        return ClipperLib::PolyFillType::pftNegative;
    else if( rule == Triangulator::FillRule::NonZero )
        return ClipperLib::PolyFillType::pftNonZero;
    else if( rule == Triangulator::FillRule::Positive )
        return ClipperLib::PolyFillType::pftPositive;
    return ClipperLib::PolyFillType::pftEvenOdd;
}
