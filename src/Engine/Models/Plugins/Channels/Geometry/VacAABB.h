#pragma once

#include "Mathematics/Rect.h"

namespace bv { namespace model {

class IVertexAttributesChannel;

// ******************************
// Calculates axis-aligned bounding box of connected components in VAC. If success returns true.
bool				AABB( const IVertexAttributesChannel * vac, const glm::mat4 & trans, mathematics::Rect * rect );

} // model
} // bv

