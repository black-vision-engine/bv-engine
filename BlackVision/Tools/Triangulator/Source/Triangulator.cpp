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
void                        FreePolyline        ( Polyline & poly );

ClipperLib::PolyFillType    GetFillRule         ( Triangulator::FillRule rule );


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
    ,   m_fillRule( FillRule::EvenOdd )
{
	ProcessContours();
}

// ***********************
//
Triangulator::Triangulator( ContoursList && contours, const std::string & debugFileName )
	:	m_contoursList( std::move( contours ) )
	,	m_printContoursToFile( true )
	,	m_fileName( debugFileName )
    ,   m_fillRule( FillRule::EvenOdd )
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
    ,   m_fillRule( FillRule::EvenOdd )
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
    m_contoursIncuding.resize( ftContourCount );
    m_contoursNesting.resize( ftContourCount, 0 );

    for( int i = 0; i < ftContourCount; ++i )
    {
        m_contoursIncuding[ i ].resize( ftContourCount, false );
    }


    //// Compute contour nesting.
    //for( int i = 0; i < ftContourCount; i++ )
    //{
    //    auto & c1 = m_contoursList[ i ];
    //    int contourNesting = 0;

    //    // 1. Find the leftmost point.
    //    FTPoint leftmost( 65536.0, 0.0 );

    //    for( size_t n = 0; n < c1->PointCount(); n++ )
    //    {
    //        FTPoint p = c1->Point( n );
    //        if( p.X() < leftmost.X() )
    //        {
    //            leftmost = p;
    //        }
    //    }

    //    // 2. Count how many other contours we cross when going further to
    //    // the left.
    //    for( int j = 0; j < ftContourCount; j++ )
    //    {
    //        if( j == i )
    //        {
    //            continue;
    //        }

    //        auto & c2 = m_contoursList[ j ];
    //        int parity = 0;

    //        if( !c1->Intersects( c2.get() ) )
    //            continue;

    //        for( size_t n = 0; n < c2->PointCount(); n++ )
    //        {
    //            FTPoint p1 = c2->Point( n );
    //            FTPoint p2 = c2->Point( ( n + 1 ) % c2->PointCount() );

    //            /* FIXME: combinations of >= > <= and < do not seem stable */
    //            if( ( p1.Y() < leftmost.Y() && p2.Y() < leftmost.Y() )
    //                || ( p1.Y() >= leftmost.Y() && p2.Y() >= leftmost.Y() )
    //                || ( p1.X() > leftmost.X() && p2.X() > leftmost.X() ) )
    //            {
    //                continue;
    //            }
    //            else if( p1.X() < leftmost.X() && p2.X() < leftmost.X() )
    //            {
    //                parity++;
    //            }
    //            else
    //            {
    //                FTPoint* top = nullptr;
    //                FTPoint* bottom = nullptr;
    //                
    //                if( p1.Y() > p2.Y() )
    //                {
    //                    top = &p1;
    //                    bottom = &p2;
    //                }
    //                else
    //                {
    //                    top = &p2;
    //                    bottom = &p1;
    //                }

    //                FTPoint a = *bottom - leftmost;
    //                FTPoint b = *top - *bottom;

    //                auto determinant = a.X() * b.Y() - a.Y() * b.X();
    //                if( determinant < 0 )
    //                {
    //                    // Sign of determinant of matrix created from vectors a and b.
    //                    parity++;
    //                }
    //                else if( determinant == 0 )
    //                {
    //                    // Point on segment.
    //                    throw new std::runtime_error( "[Triangulator] Internal contour point lies on outer contour." );
    //                }
    //                else
    //                {
    //                    // determinant > 0
    //                    { parity; }
    //                    a = b;
    //                }
    //            }
    //        }

    //        // We determine if our contour c1 is inside contour c2. If c1 is inside, that means
    //        // we must add level of nesting to variable contourNesting for c1.
    //        contourNesting += parity % 2;
    //        // Contour i is included by j.
    //        m_contoursIncuding[ j ][ i ] = parity % 2 != 0;   // != used to avoid warning C4800.
    //    }

    //    m_contoursNesting[ i ] = contourNesting;

    //    // Contours orientation doesn't match nesting parity.
    //    if( c1->IsClockwise() != ( contourNesting % 2 == 0 ) )
    //        c1->InverseOrientation();
    //}
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
    m_selfIntersections.resize( ftContourCount );

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

        //Polyline & polyline = m_polylines[ c ];
        //polyline.reserve( contour->PointCount() );
        polylinesPaths[ c ].reserve( contour->PointCount() );

        for( size_t p = 0; p < contour->PointCount(); ++p )
        {
            //p2t::Point * d = new p2t::Point( contour->Point( p ).X(), contour->Point( p ).Y() );
            //polyline.push_back( d );
            polylinesPaths[ c ] << ClipperLib::IntPoint( static_cast< ClipperLib::cInt >( contour->Point( p ).X() * scaleFloat ), static_cast< ClipperLib::cInt >( contour->Point( p ).Y() * scaleFloat ) );
        }

        //PolylineValidator validator( std::move( polyline ) );
        //validator.FindSelfIntersections();
        //validator.DecomposeContour();

        //m_selfIntersections.push_back( validator.StealIntersections() );
        //polyline = HeuristicFindMainContour( validator.StealDecomposedPolylines() );    // Heuristic: Take longest contour ;)
        ////m_selfIntersections.push_back( Polyline() );
    }

    SetFillRule( FillRule::NonZero );

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


	// Print contours to file for debug purposes.
	if( m_printContoursToFile )
	{
        //PrintToFileAsUnitTest();
        PrintContoursToFile();
	}

    for( auto & outerContour : resultTree.Childs )
    {
        TriangulateHierarchy( *outerContour, mesh, scaleFloat );
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
            for( int j = 0; j < childCount; ++j )
            {
                if( !paths[ j + 1 ].empty() )
                {
                    ClipperLib::Paths newPaths;

                    ClipperLib::ClipperOffset offseter;
                    offseter.AddPath( paths[ j + 1 ], ClipperLib::JoinType::jtSquare, ClipperLib::EndType::etClosedPolygon );
                    offseter.Execute( newPaths, -distance );

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

// ***********************
//
void        FreePolyline        ( Polyline & poly )
{
    for( auto & point : poly )
        delete point;
    poly.clear();
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
    // Including
    file << "Including: " << std::endl;
    for( int i = 0; i < m_contoursIncuding.size(); ++i )
    {
        for( int j = 0; j < m_contoursIncuding.size(); ++j )
        {
            if( m_contoursIncuding[ i ][ j ] )
                file << "true,\t";
            else
                file << "false,\t";
        }
        file << std::endl;
    }
    file << std::endl;

    // ***********************
    // Nesting
    file << "Nesting: " << std::endl;
    for( int i = 0; i < m_contoursNesting.size(); ++i )
    {
        file << m_contoursNesting[ i ] << ", ";
    }
    file << std::endl;


// ***********************
// Polylines
    for( size_t c = 0; c < m_polylines.size(); c++ )
    {
        file << std::endl << "Contour number " << c << std::endl;
        file << "Nesting: " << m_contoursNesting[ c ] << std::endl;
        file << "Is clockwise: " << m_contoursList[ c ]->IsOuterContour() << std::endl;

        // ***********************
        // Intersections
        if( !m_selfIntersections[ c ].empty() )
        {
            file << "Self Intersections in pre computing phase: ";
            
            for( int i = 0; i < m_selfIntersections[ c ].size(); ++i )
            {
                auto point = m_selfIntersections[ c ][ i ];
                file << "( " << point->x << ", " << point->y << " ) ";
            }

            file << std::endl;
        }

        // ***********************
        // Including
        file << "Includes contours: ";
        for( int i = 0; i < m_polylines.size(); ++i )
        {
            if( m_contoursIncuding[ c ][ i ] )
            {
                file << i << " ";
            }
        }
        file << std::endl << std::endl;

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
Polyline &&         Triangulator::HeuristicFindMainContour  ( PolylinesVec && polylines )
{
    assert( !polylines.empty() );
    assert( !polylines[ 0 ].empty() );

    int longestIdx = 0;
    int longestLength = 0;

    for( int i = 0; i < polylines.size(); ++i )
    {
        if( longestLength < polylines[ i ].size() )
        {
            longestIdx = i;
            longestLength = (int)polylines[ i ].size();
        }
    }

    // Delete rejected polylines. Dealocate points.
    for( int i = 0; i < polylines.size(); ++i )
    {
        if( i != longestIdx )
        {
            for( auto & point : polylines[ i ] )
            {
                delete point;
            }
            polylines[ i ].clear();
        }
    }


    return std::move( polylines[ longestIdx ] );
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

// ***********************
//
void                Triangulator::PrintToFileAsUnitTest()
{
    std::fstream file( m_fileName, std::ios_base::app );
    assert( !file.fail() );

    // ***********************
    // Header - beginning of shape
    file << std::endl << std::endl << "SECTION( \"Loading file : [" << m_contourName.c_str() << "]\")\n{\n";

    // ***********************
    // Contours sizes
    file << "\tint contourSizeArray[] = { ";
    for( int i = 0; i < m_polylines.size(); ++i )
    {
        file << m_polylines[ i ].size();
        
        if( i != m_polylines.size() - 1 )
            file << ", ";
    }
    file << " };" << std::endl;


    // ***********************
    // Nesting
    file << "\tint nestingArray[] = { ";
    for( int i = 0; i < m_contoursNesting.size(); ++i )
    {
        file << m_contoursNesting[ i ];

        if( i != m_contoursNesting.size() - 1 )
            file << ", ";
    }
    file << " };" << std::endl;


    // ***********************
    // Intersection sizes
    size_t numIntersects = 0;

    file << "\tint intersectsSizes[] = { ";
    for( int i = 0; i < m_selfIntersections.size(); ++i )
    {
        numIntersects += m_selfIntersections[ i ].size();
        file << m_selfIntersections[ i ].size();

        if( i != m_selfIntersections.size() - 1 )
            file << ", ";
    }
    file << " };" << std::endl;

    // ***********************
    // Intersections data
    if( numIntersects )
    {
        int intersectsCounter = 0;
        file << "\tglm::vec2 intersectsArray[] = { ";
        for( int i = 0; i < m_selfIntersections.size(); ++i )
        {
            for( int j = 0; j < m_selfIntersections[ i ].size(); ++j )
            {
                auto point = m_selfIntersections[ i ][ j ];
                file << "glm::vec2( " << point->x << ", " << point->y << " )";

                if( intersectsCounter != numIntersects - 1 )
                    file << ", ";

                intersectsCounter++;
            }
        }
        file << " };" << std::endl << std::endl;
    }
    else
    {
        file << "\tglm::vec2* intersectsArray = nullptr;" << std::endl << std::endl;
    }

    // ***********************
    // Including
    file << "\tbool includingArray[] =\n\t{\n";
    for( int i = 0; i < m_contoursIncuding.size(); ++i )
    {
        file << "\t\t";
        for( int j = 0; j < m_contoursIncuding.size(); ++j )
        {
            if( m_contoursIncuding[ i ][ j ] )
                file << "true";
            else
                file << "false";

            if( i != m_contoursIncuding.size() - 1 || j != m_contoursIncuding.size() - 1 )
                file << ",\t";
        }
        file << std::endl;
    }
    file << "\t};" << std::endl << std::endl;

    // Call test function and add closing brace.
    file << "\tTestFileWithArrays( \"" << m_contourName.c_str() << "\", plugin, triangulate, contourSizeArray, nestingArray, includingArray, intersectsSizes, intersectsArray );" << std::endl;
    file << "}" << std::endl << std::endl;

}

