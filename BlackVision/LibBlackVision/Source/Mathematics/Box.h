#pragma once

#include "CoreDEF.h"
#include "Mathematics/glm_inc.h"

#include <algorithm>


namespace bv { namespace mathematics {

struct Box;

DEFINE_PTR_TYPE( Box )
DEFINE_CONST_PTR_TYPE( Box )

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

    void	Include					( const glm::vec3 & point )
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

    //Float32		Width				() const	{ return xmax - xmin; }
    //Float32		Height				() const	{ return ymax - ymin; }
};

} // mathematics
} // bv