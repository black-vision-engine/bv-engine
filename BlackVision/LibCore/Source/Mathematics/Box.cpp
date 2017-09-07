#include "stdafxCore.h"

#include "Box.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace mathematics {

namespace
{

// ******************************
// 
inline void        ComponentWiseMinMaxVector   ( glm::vec3 srcVec1, glm::vec3 srcVec2, glm::vec3 & dstMin, glm::vec3 & dstMax )
{
    if( srcVec1.x < srcVec2.x )
    {
        dstMin.x = srcVec1.x;
        dstMax.x = srcVec2.x;
    }
    else
    {
        dstMin.x = srcVec2.x;
        dstMax.x = srcVec1.x;
    }

    if( srcVec1.y < srcVec2.y )
    {
        dstMin.y = srcVec1.y;
        dstMax.y = srcVec2.y;
    }
    else
    {
        dstMin.y = srcVec2.y;
        dstMax.y = srcVec1.y;
    }

    if( srcVec1.z < srcVec2.z )
    {
        dstMin.z = srcVec1.z;
        dstMax.z = srcVec2.z;
    }
    else
    {
        dstMin.z = srcVec2.z;
        dstMax.z = srcVec1.z;
    }
}

}   //  annonymous


// ******************************
// 
Box::Box			()
    :   xmin( 0.f ), xmax( 0.f ), 
        ymin( 0.f ), ymax( 0.f ), 
        zmin( 0.f ), zmax( 0.f), 
        m_empty( true ) { }

// ******************************
// 
Box::Box			( Float32 left, Float32 top, Float32 right, Float32 bottom, Float32 near, Float32 far )
    :   xmin( left ), xmax( right ), 
        ymin( bottom ), ymax( top ), 
        zmin( near ), zmax( far ),
        m_empty( false ) { }

// ******************************
// 
BoxPtr		Box::Create( )
{
    return std::make_shared< Box >();
}

// ******************************
// 
void        Box::Include					( const glm::vec3 & point )
{
    if( m_empty )
    {
        m_empty = false;
        xmin = point.x;
        ymin = point.y;
        xmax = point.x;
        ymax = point.y;
        zmin = point.z;
        zmax = point.z;
    }
    else
    {
        xmin = std::min( xmin, point.x );
        ymin = std::min( ymin, point.y );
        zmin = std::min( zmin, point.z );
        xmax = std::max( xmax, point.x );
        ymax = std::max( ymax, point.y );
        zmax = std::max( zmax, point.z );
    }
}

// ******************************
//
void        Box::Include		            ( glm::vec3 && point )
{
    if( m_empty )
    {
        m_empty = false;
        xmin = point.x;
        ymin = point.y;
        xmax = point.x;
        ymax = point.y;
        zmin = point.z;
        zmax = point.z;
    }
    else
    {
        xmin = std::min( xmin, point.x );
        ymin = std::min( ymin, point.y );
        zmin = std::min( zmin, point.z );
        xmax = std::max( xmax, point.x );
        ymax = std::max( ymax, point.y );
        zmax = std::max( zmax, point.z );
    }
}

// ***********************
// Parameter rayDirection should be normalized ray vector
// Returns distance to intersection point and infinity or -infinity if there's no intersection.
// If box is behind rayPoint return value is negative.
Float32   Box::RayIntersection         ( glm::vec3 rayPoint, glm::vec3 rayDirection, bool & inside ) const
{
    if( rayDirection.x == 0.0 )
        rayDirection.x = std::numeric_limits< float >::epsilon();
    if( rayDirection.y == 0.0 )
        rayDirection.y = std::numeric_limits< float >::epsilon();
    if( rayDirection.z == 0.0 )
        rayDirection.z = std::numeric_limits< float >::epsilon();


    glm::vec3 inverseRayDir = glm::vec3( 1.0 / rayDirection.x, 1.0 / rayDirection.y, 1.0 / rayDirection.z );
        
    // Compute distances from rayPoint to minimal box corner and maximal box corner.
    glm::vec3 minBoxPointDist = inverseRayDir * ( glm::vec3( xmin, ymin, zmin ) - rayPoint );
    glm::vec3 maxBoxPointDist = inverseRayDir * ( glm::vec3( xmax, ymax, zmax ) - rayPoint );

    // Compute distances to nearest planes.
    glm::vec3 nearestPlanesDist;
    glm::vec3 farthestPlanesDist;
    ComponentWiseMinMaxVector( minBoxPointDist, maxBoxPointDist, nearestPlanesDist, farthestPlanesDist );

    // We have distances of 3 nearest planes of the box (nearestPlanesDist). The furthest from theese
    // planes should be point of intersection, if this intersection exist.
    // Intersection exists only if neither of the farthestPlanesDist is nearer than any of nearestPlanesDist.
    Float32 maxMinPlane = glm::max( glm::max( nearestPlanesDist.x, nearestPlanesDist.y ), nearestPlanesDist.z );
    Float32 minMaxPlane = glm::min( glm::min( farthestPlanesDist.x, farthestPlanesDist.y ), farthestPlanesDist.z );

    // Box is behind rayPoint.
    if( minMaxPlane < 0.0f )
    {
        // No intersection behind rayPoint
        if( maxMinPlane > minMaxPlane )
            return -std::numeric_limits< Float32 >::infinity();

        // Returns intersection dinstance behind raypoint.
        return minMaxPlane;
    }

    // No intersection in front of rayPoint
    if( maxMinPlane > minMaxPlane )
        return std::numeric_limits< Float32 >::infinity();

    if( minMaxPlane > 0.0f && maxMinPlane < 0.0f )
        inside = true;

    // Returns intersection distance.
    return maxMinPlane;
}


// ******************************
// 
void        Box::Include            ( const Box & box )
{
    if( !box.m_empty )
    {
        this->Include( glm::vec3( box.xmin, box.ymin, box.zmin ) );
        this->Include( glm::vec3( box.xmax, box.ymax, box.zmax ) );
    }
}

// ******************************
// 
Float32		Box::Width				() const	
{
    return xmax - xmin;
}

// ******************************
// 
Float32		Box::Height				() const
{
    return ymax - ymin;
}

// ******************************
// 
Float32     Box::Depth               () const
{
    return zmax - zmin;
}

// ***********************
//
glm::vec3   Box::Center             () const
{
    glm::vec3 center;
    center.x = ( xmin + xmax ) / 2;
    center.y = ( ymin + ymax ) / 2;
    center.z = ( zmin + zmax ) / 2;
    return center;
}

// ***********************
//
void        Box::Transform          ( const glm::mat4 & transformation )
{
    glm::vec4 BBPoints[ 8 ];
    
    BBPoints[ 0 ] = glm::vec4( xmin, ymin, zmin, 1.0 );
    BBPoints[ 1 ] = glm::vec4( xmin, ymax, zmin, 1.0 );
    BBPoints[ 2 ] = glm::vec4( xmax, ymax, zmin, 1.0 );
    BBPoints[ 3 ] = glm::vec4( xmax, ymin, zmin, 1.0 );

    BBPoints[ 4 ] = glm::vec4( xmin, ymin, zmax, 1.0 );
    BBPoints[ 5 ] = glm::vec4( xmin, ymax, zmax, 1.0 );
    BBPoints[ 6 ] = glm::vec4( xmax, ymax, zmax, 1.0 );
    BBPoints[ 7 ] = glm::vec4( xmax, ymin, zmax, 1.0 );

    for( int i = 0; i < 8; ++i )
    {
        BBPoints[ i ] = transformation * BBPoints[ i ];
    }

    // Reset min and max values
    xmin = BBPoints[ 0 ].x;
    ymin = BBPoints[ 0 ].y;
    zmin = BBPoints[ 0 ].z;

    xmax = BBPoints[ 0 ].x;
    ymax = BBPoints[ 0 ].y;
    zmax = BBPoints[ 0 ].z;


    for( int i = 1; i < 8; ++i )
    {
        xmin = std::min( xmin, BBPoints[ i ].x );
        ymin = std::min( ymin, BBPoints[ i ].y );
        zmin = std::min( zmin, BBPoints[ i ].z );

        xmax = std::max( xmax, BBPoints[ i ].x );
        ymax = std::max( ymax, BBPoints[ i ].y );
        zmax = std::max( zmax, BBPoints[ i ].z );
    }
}

// ******************************
// 
std::vector< glm::vec3 > Box::GetVerticies () const
{
    std::vector< glm::vec3 > ret;
    ret.reserve( 8 );

    ret.push_back( glm::vec3( this->xmin, this->ymin, this->zmin ) );
    ret.push_back( glm::vec3( this->xmin, this->ymin, this->zmax ) );
    ret.push_back( glm::vec3( this->xmin, this->ymax, this->zmax ) );
    ret.push_back( glm::vec3( this->xmax, this->ymax, this->zmax ) );
    ret.push_back( glm::vec3( this->xmax, this->ymax, this->zmin ) );
    ret.push_back( glm::vec3( this->xmax, this->ymin, this->zmin ) );
    ret.push_back( glm::vec3( this->xmin, this->ymax, this->zmin ) );
    ret.push_back( glm::vec3( this->xmax, this->ymin, this->zmax ) );

    return ret;
}

} // mathematics
} // bv