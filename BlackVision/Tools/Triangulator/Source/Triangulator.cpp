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

#include "poly2tri/poly2tri.h"

// Debug
#include <fstream>




// ================================ //
//
Triangulator::Triangulator( ContoursList && contours )
	:	m_contoursList( std::move( contours ) )
	,	m_printContoursToFile( false )
{
	ProcessContours();
}

Triangulator::Triangulator( ContoursList && contours, const std::string debugFileName )
	:	m_contoursList( std::move( contours ) )
	,	m_printContoursToFile( true )
	,	m_fileName( debugFileName )
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


    // Compute contour nesting.
    for( int i = 0; i < ftContourCount; i++ )
    {
        auto & c1 = m_contoursList[ i ];
        int contourNesting = 0;

        // 1. Find the leftmost point.
        FTPoint leftmost( 65536.0, 0.0 );

        for( size_t n = 0; n < c1->PointCount(); n++ )
        {
            FTPoint p = c1->Point( n );
            if( p.X() < leftmost.X() )
            {
                leftmost = p;
            }
        }

        // 2. Count how many other contours we cross when going further to
        // the left.
        for( int j = 0; j < ftContourCount; j++ )
        {
            if( j == i )
            {
                continue;
            }

            auto & c2 = m_contoursList[ j ];
            int parity = 0;

            if( !c1->Intersects( c2.get() ) )
                continue;

            for( size_t n = 0; n < c2->PointCount(); n++ )
            {
                FTPoint p1 = c2->Point( n );
                FTPoint p2 = c2->Point( ( n + 1 ) % c2->PointCount() );

                /* FIXME: combinations of >= > <= and < do not seem stable */
                if( ( p1.Y() < leftmost.Y() && p2.Y() < leftmost.Y() )
                    || ( p1.Y() >= leftmost.Y() && p2.Y() >= leftmost.Y() )
                    || ( p1.X() > leftmost.X() && p2.X() > leftmost.X() ) )
                {
                    continue;
                }
                else if( p1.X() < leftmost.X() && p2.X() < leftmost.X() )
                {
                    parity++;
                }
                else
                {
                    FTPoint a = p1 - leftmost;
                    FTPoint b = p2 - leftmost;
                    if( b.X() * a.Y() <= b.Y() * a.X() )
                    {
                        // Check cross product between vectors (on plane, which means result is only z value).
                        parity++;
                    }
                }
            }

            // We determine if our contour c1 is inside contour c2. If c1 is inside, that means
            // we must add level of nesting to variable contourNesting for c1.
            contourNesting += parity % 2;
            // Contour i is included by j.
            m_contoursIncuding[ j ][ i ] = parity % 2 != 0;   // != used to avoid warning C4800.
        }

        m_contoursNesting[ i ] = contourNesting;

    }
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

    m_polylines.resize( ftContourCount );
    m_selfIntersections.reserve( ftContourCount );

    for( size_t c = 0; c < ContourCount(); ++c )
    {
        const auto & contour = m_contoursList[ c ];

        Polyline& polyline = m_polylines[ c ];
        polyline.reserve( contour->PointCount() );

        for( size_t p = 0; p < contour->PointCount(); ++p )
        {
            p2t::Point * d = new p2t::Point( contour->Point( p ).X(), contour->Point( p ).Y() );
            polyline.push_back( d );
        }

        PolylineValidator validator( polyline );
        auto & intersectionPoints = validator.FindSelfIntersections();
        m_selfIntersections.push_back( intersectionPoints );
    }

	// Print contours to file for debug purposes.
	if( m_printContoursToFile )
	{
        PrintContoursToFile();
	}



    // Process only outer contours. Check intersections with rest countours and add as holes
    // only this ones, that are inside bounding box. 
    for( size_t i = 0; i < ContourCount(); ++i )
    {
        const auto & c1 = m_contoursList[ i ];
        // We make meshes only for outer contours.
        if( !c1->IsOuterContour() )
            continue;

        //if( !( m_contoursNesting[ i ] % 2 ) )
        //    continue;

        int c1Nesting = m_contoursNesting[ i ];
        p2t::CDT * cdt = new p2t::CDT( m_polylines[ i ] );

        for( size_t c = 0; c < ContourCount(); ++c )
        {
            const auto & c2 = m_contoursList[ c ];
            int c2Nesting = m_contoursNesting[ c ];
            // Hole can be only inner contour.
            if( c2->IsOuterContour() )
                continue;

            // Contour c1 (index i) includes contour c2 (index c).
            // We add hole only when c2 is direct hole of c1 (second condition of if statement).
            if( m_contoursIncuding[ i ][ c ] &&
                c1Nesting == c2Nesting - 1 )
            {
                cdt->AddHole( m_polylines[ c ] );
            }
        }

        cdt->Triangulate();

        mesh.Begin();

        for( auto t : cdt->GetTriangles() )
        {
            auto p0 = t->GetPoint( 0 );
            auto p1 = t->GetPoint( 1 );
            auto p2 = t->GetPoint( 2 );
            mesh.AddPoint( (float)p0->x, (float)p0->y, 0.0 );
            mesh.AddPoint( (float)p1->x, (float)p1->y, 0.0 );
            mesh.AddPoint( (float)p2->x, (float)p2->y, 0.0 );
        }

        mesh.End();

        delete cdt;
    }

	return mesh;
}

// ================================ //
//
void Triangulator::PrintContoursToFile()
{
    std::fstream file( m_fileName, std::ios_base::app );
    assert( !file.fail() );

    file << std::endl << std::endl << "Next shape" << std::endl;

    for( size_t c = 0; c < m_polylines.size(); c++ )
    {
        file << std::endl << "Contour number " << c << std::endl;
        file << "Nesting: " << m_contoursNesting[ c ] << std::endl;
        file << "Is clockwise: " << m_contoursList[ c ]->IsOuterContour() << std::endl;

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

        file << "Includes contours: ";
        for( int i = 0; i < m_polylines.size(); ++i )
        {
            if( m_contoursIncuding[ c ][ i ] )
            {
                file << i << " ";
            }
        }
        file << std::endl << std::endl;

        auto & contour = m_polylines[ c ];
        for( size_t i = 0; i < m_polylines[ c ].size(); i++ )
        {
            file << contour[ i ]->x << " " << contour[ i ]->y << std::endl;
        }
    }

    file.close();
}

