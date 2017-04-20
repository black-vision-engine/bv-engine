#pragma once

#include "CoreDEF.h"
#include "Mathematics/glm_inc.h"

#include <algorithm>

#undef min
#undef max

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

	bool	m_empty;

	Rect			()
		: xmin( 0.f ), xmax( 0.f ), ymin( 0.f ), ymax( 0.f ), m_empty( true ) { }

	Rect			( Float32 left, Float32 top, Float32 right, Float32 bottom )
		: xmin( left ), xmax( right ), ymin( top ), ymax( bottom ), m_empty( false ) { }

	static	RectPtr		Create( )
	{
		return std::make_shared< Rect >();
	}

	static	RectPtr		Create( Float32 left, Float32 top, Float32 right, Float32 bottom )
	{
		return std::make_shared< Rect >( left, top, right, bottom );
	}

	void	Include					( const glm::vec2 & point )
	{
		if( m_empty )
		{
			m_empty = false;
			xmin = point.x;
			ymin = point.y;
			xmax = point.x;
			ymax = point.y;
		}
		else
		{
			xmin = std::min( xmin, point.x );
			ymin = std::min( ymin, point.y );
			xmax = std::max( xmax, point.x );
			ymax = std::max( ymax, point.y );
		}
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
		return	(	xmin < rect.xmax && xmax > rect.xmin &&
					ymin < rect.ymax && ymax > rect.ymin
				);
	}

	Float32		Width				() const	{ return xmax - xmin; }
	Float32		Height				() const	{ return ymax - ymin; }
};

} // mathematics
} // bv