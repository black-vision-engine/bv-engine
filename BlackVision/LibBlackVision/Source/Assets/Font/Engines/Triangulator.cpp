#include "stdafx.h"
#include "Triangulator.h"


#include "FTVectoriser.h"

#include "poly2tri.h"


typedef float GLfloat;
#define GL_TRIANGLES                      0x0004



Triangulator::Triangulator( const FT_GlyphSlot glyph )
    : contourList( 0 ),
    mesh( 0 ),
    ftContourCount( 0 ),
    contourFlag( 0 )
{
    if( glyph )
    {
        outline = glyph->outline;

        ftContourCount = outline.n_contours;
        contourList = 0;
        contourFlag = outline.flags;

        ProcessContours();
    }
}


Triangulator::~Triangulator()
{
    for( size_t c = 0; c < ContourCount(); ++c )
    {
        delete contourList[ c ];
    }

    delete[] contourList;
    delete mesh;
}


void Triangulator::ProcessContours()
{
    short contourLength = 0;
    short startIndex = 0;
    short endIndex = 0;

    contourList = new FTContour*[ ftContourCount ];
    contoursIncuding.resize( ftContourCount );
    contoursNesting.resize( ftContourCount, 0 );

    for( int i = 0; i < ftContourCount; ++i )
    {
        contoursIncuding[ i ].resize( ftContourCount, false );
    }


    for( int i = 0; i < ftContourCount; ++i )
    {
        FT_Vector* pointList = &outline.points[ startIndex ];
        char* tagList = &outline.tags[ startIndex ];

        endIndex = outline.contours[ i ];
        contourLength = ( endIndex - startIndex ) + 1;

        FTContour* contour = new FTContour( pointList, tagList, contourLength );

        contourList[ i ] = contour;

        startIndex = endIndex + 1;
    }

    auto orient = FT_Outline_Get_Orientation( &outline );

    // Compute contour nesting.
    int contourNesting = 0;
    for( int i = 0; i < ftContourCount; i++ )
    {
        FTContour *c1 = contourList[ i ];

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

            FTContour *c2 = contourList[ j ];
            int parity = 0;

            if( !c1->Intersects( c2 ) )
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
                    if( b.X() * a.Y() > b.Y() * a.X() )
                    {
                        parity++;
                    }
                }
            }

            // We determine if our contour c1 is inside contour c2. If c1 is inside, that means
            // we must add level of nesting to variable contourNesting for c1.
            contourNesting += parity % 2;
            // Contour i is included by j.
            contoursIncuding[ j ][ i ] = parity % 2 != 0;   // != used to avoid warning C4800.
        }

        // Make sure the glyph has the proper orientation.
        // Some formats use CCW order and other 
        bool inverse = false;
        if( orient == FT_ORIENTATION_POSTSCRIPT )
            inverse = true;

        c1->SetParity( inverse );
        contoursNesting[ i ] = contourNesting;

    }
}


size_t Triangulator::PointCount()
{
    size_t s = 0;
    for( size_t c = 0; c < ContourCount(); ++c )
    {
        s += contourList[ c ]->PointCount();
    }

    return s;
}


const FTContour* const Triangulator::Contour( size_t index ) const
{
    return ( index < ContourCount() ) ? contourList[ index ] : NULL;
}


void Triangulator::MakeMesh( FTGL_DOUBLE /*zNormal*/, int outsetType, float outsetSize )
{
    if( mesh )
    {
        delete mesh;
    }

    mesh = new FTMesh;

    std::vector< std::vector< p2t::Point * > > contoursVecPointsVec;
    contoursVecPointsVec.resize( ftContourCount );


    for( size_t c = 0; c < ContourCount(); ++c )
    {
        const FTContour* contour = contourList[ c ];

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
    for( int i = 0; i < ContourCount(); ++i )
    {
        const FTContour* c1 = contourList[ i ];
        // We make meshes only for outer contours.
        if( !c1->IsOuterContour() )
            break;

        int c1Nesting = contoursNesting[ i ];
        p2t::CDT * cdt = new p2t::CDT( contoursVecPointsVec[ i ] );

        for( int c = 0; c < ContourCount(); ++c )
        {
            const FTContour* c2 = contourList[ c ];
            int c2Nesting = contoursNesting[ c ];
            // Hole can be only inner contour.
            if( c2->IsOuterContour() )
                break;

            // Contour c1 (index i) includes contour c2 (index c).
            // We add hole only when c2 is direct hole of c1 (second condition of if statement).
            if( contoursIncuding[ i ][ c ] &&
                c1Nesting == c2Nesting - 1 )
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

