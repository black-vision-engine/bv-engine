#pragma once

#include "glm_inc.h"

namespace bv { 
    
namespace mathematics {
struct Box;
} // mathematics

class BoundingVolume {
public:
    glm::mat4               m_transform; // apply unit cube to get bounding box :)

                            BoundingVolume          ( const mathematics::Box & );
};

} // bv
