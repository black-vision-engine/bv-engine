#pragma once

#include "Mathematics/glm_inc.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace bv {

class SQTTransform
{
public:

                    SQTTransform        ();
    glm::mat4x4     Evaluate            ( const glm::vec3 & translation, const glm::quat & rotation, const glm::vec3 & scale, const glm::vec3 & center ) const;

};

}