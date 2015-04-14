#pragma once

#include "CoreDEF.h"

namespace bv {

class OffsetState
{
public:

    // Set whether offset should be enabled for the various polygon drawing
    // modes (fill, line, point).
    bool fillEnabled;   // false
    bool linesEnabled;  // false
    bool pointsEnabled; // false

    // The offset is Scale*dZ + Bias*r where dZ is the change in depth
    // relative to the screen space area of the poly, and r is the smallest
    // resolvable depth difference.  Negative values move polygons closer to
    // the eye.
    float scale;  // 0
    float bias;   // 0

public:

    OffsetState  ();

};

DEFINE_PTR_TYPE(OffsetState)
DEFINE_CONST_PTR_TYPE(OffsetState)

} //bv
