#pragma once 

#include "Mathematics/Box.h"

namespace bv { namespace mathematics {

class TransformationUtils
{
public:
     static Box            Transform( const Box * box, const glm::mat4 & tx );

};

} // mathematics
} // bv