#pragma once

#include "System/BasicTypes.h"
#include "glm/glm.hpp"

#include <algorithm>


namespace bv { namespace mathematics {

struct Rect;

DEFINE_PTR_TYPE( Rect )
DEFINE_CONST_PTR_TYPE( Rect )

struct Rect
{
	Float32	xmin;
	Float32	xmax;
	Float32	ymin;
	Float32	ymax;


	Rect			()
		: xmin( 0.f ), xmax( 0.f ), ymin( 0.f ), ymax( 0.f ) { }

	Rect			( Float32 left, Float32 top, Float32 right, Float32 bottom )
		: xmin( left ), xmax( right ), ymin( top ), ymax( bottom ) { }

	static	RectPtr		Create( Float32 left = 0.f, Float32 top = 0.f, Float32 right = 0.f, Float32 bottom  = 0.f )
	{
		return std::make_shared< Rect >( left, top, right, bottom );
	}

	void	Include					( const glm::vec2 & point )
	{
		xmin = std::min( xmin, point.x );
		ymin = std::min( ymin, point.y );
		xmax = std::max( xmax, point.x );
		ymax = std::max( ymax, point.y );
	}

	void	Include					( const Rect & rect )
	{
		Include( glm::vec2( rect.xmin, rect.ymin ) );
		Include( glm::vec2( rect.xmin, rect.ymax ) );
		Include( glm::vec2( rect.xmax, rect.ymin ) );
		Include( glm::vec2( rect.xmax, rect.ymax ) );
	}

	void	Transform				( const glm::mat4 & tr )	
	{
		Rect r;
		r.Include( glm::vec2( glm::vec4( xmin, ymin, 0.f, 1.f ) * tr ) );
		r.Include( glm::vec2( glm::vec4( xmin, ymax, 0.f, 1.f ) * tr ) );
		r.Include( glm::vec2( glm::vec4( xmax, ymin, 0.f, 1.f ) * tr ) );
		r.Include( glm::vec2( glm::vec4( xmax, ymax, 0.f, 1.f ) * tr ) );

		this->xmin = r.xmin;
		this->xmax = r.xmax;
		this->ymin = r.ymin;
		this->ymax = r.ymax;
	}

	bool	IsInside				( const glm::vec2 & point ) const
	{
		return point.x >= this->xmin && point.x <= this->xmax && point.y >= this->ymin && point.y <= this->ymax;
	}

	bool	HasNonEmptyIntersection	( const Rect & rect ) const
	{
		return		IsInside( glm::vec2( rect.xmin, rect.ymin ) )
				||	IsInside( glm::vec2( rect.xmin, rect.ymax ) )
				||	IsInside( glm::vec2( rect.xmax, rect.ymin ) )
				||	IsInside( glm::vec2( rect.xmax, rect.ymax ) );
	}

	Float32		Width				() const	{ return xmax - xmin; }
	Float32		Height				() const	{ return ymax - ymin; }
};

} // mathematics
} // bv