#include "stdafx.h"

#include "BoundingVolume.h"

#include "Box.h"

#include <glm/gtc/matrix_transform.hpp>

namespace bv {

BoundingVolume::BoundingVolume          ( const mathematics::Box & box )
{
    m_transform *= glm::translate( glm::mat4( 1.0f ), glm::vec3( box.xmin, box.ymin, box.zmin ) );
    m_transform *= glm::scale( glm::mat4( 1.0f ), glm::vec3( box.xmax - box.xmin, box.ymax - box.ymin, box.zmax - box.zmin ) );
}

}
