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

                        Box			    ();

                        Box			    ( Float32 left, Float32 top, Float32 right, Float32 bottom, Float32 near, Float32 far );

    static	BoxPtr		Create          ();

    void                Include		    ( const glm::vec3 & point );

    Float32             RayIntersection ( glm::vec3 rayPoint, glm::vec3 rayDirection ) const;

    Float32		        Width		    () const;
    Float32		        Height			() const;
    Float32             Depth           () const;
};

} // mathematics
} // bv