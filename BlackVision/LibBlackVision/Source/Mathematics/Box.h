#pragma once

#include "CoreDEF.h"
#include "Mathematics/glm_inc.h"

#include <algorithm>

#undef min // FIXME properly
#undef max // FIXME properly

namespace bv { namespace mathematics {

struct Box;

DEFINE_PTR_TYPE( Box )
DEFINE_CONST_PTR_TYPE( Box )

namespace
{

inline void        ComponentWiseMinMaxVector   ( glm::vec3 srcVec1, glm::vec3 srcVec2, glm::vec3 & dstMin, glm::vec3 & dstMax );

}   //  annonymous



struct Box
{
    Float32	xmin;
    Float32	xmax;
    Float32	ymin;
    Float32	ymax;
    Float32 zmin;
    Float32 zmax;

    bool	m_empty;

    Box			()
        :   xmin( 0.f ), xmax( 0.f ), 
            ymin( 0.f ), ymax( 0.f ), 
            zmin( 0.f ), zmax( 0.f), 
            m_empty( true ) { }

    Box			( Float32 left, Float32 top, Float32 right, Float32 bottom, Float32 near, Float32 far )
        :   xmin( left ), xmax( right ), 
            ymin( top ), ymax( bottom ), 
            zmin( near ), zmax( far ),
            m_empty( false ) { }

    static	BoxPtr		Create( )
    {
        return std::make_shared< Box >();
    }

    //static	BoxPtr		Create( Float32 left, Float32 top, Float32 right, Float32 bottom, Float32 near, Float32 far )
    //{
    //	return std::make_shared< Box >( left, top, right, bottom, near, far );
    //}

    void        Include					( const glm::vec3 & point )
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
    Float32   RayIntersection         ( glm::vec3 rayPoint, glm::vec3 rayDirection )
    {
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

        // Returns intersection distance.
        return maxMinPlane;
    }

    //void	Include					( const Box & Box )
    //{
    //	Include( glm::vec3( Box.xmin, Box.ymin, Box.zmin ) );
    //	Include( glm::vec3( Box.xmin, Box.ymax ) );
    //	Include( glm::vec3( Box.xmax, Box.ymin ) );
    //	Include( glm::vec3( Box.xmax, Box.ymax ) );
    //}

    //void	Transform				( const glm::mat4 & tr )	
    //{
    //	Box r;
    //	r.Include( glm::vec2( glm::vec4( xmin, ymin, 0.f, 1.f ) * tr ) );
    //	r.Include( glm::vec2( glm::vec4( xmin, ymax, 0.f, 1.f ) * tr ) );
    //	r.Include( glm::vec2( glm::vec4( xmax, ymin, 0.f, 1.f ) * tr ) );
    //	r.Include( glm::vec2( glm::vec4( xmax, ymax, 0.f, 1.f ) * tr ) );

    //	this->xmin = r.xmin;
    //	this->xmax = r.xmax;
    //	this->ymin = r.ymin;
    //	this->ymax = r.ymax;
    //}

    //bool	IsInside				( const glm::vec2 & point ) const
    //{
    //	return point.x >= this->xmin && point.x <= this->xmax && point.y >= this->ymin && point.y <= this->ymax;
    //}

    //bool	HasNonEmptyIntersection	( const Box & Box ) const
    //{
    //	return	(	xmin < Box.xmax && xmax > Box.xmin &&
    //				ymin < Box.ymax && ymax > Box.ymin
    //			);
    //}

    Float32		Width				() const	{ return xmax - xmin; }
    Float32		Height				() const	{ return ymax - ymin; }
    Float32     Depth               () const    { return zmax - zmin; }
};

namespace
{

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

} // mathematics
} // bv