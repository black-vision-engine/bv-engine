#pragma once

#include "Mathematics/Rect.h"

#include "Mathematics/glm_inc.h"

#include <vector>

namespace bv { namespace mathematics {

// ******************************
// Calculates axis-aligned bounding box of connected components in VAC. If success returns true.
Rect					AABB( const std::vector< glm::vec2 > & points );

} // model
} // bv