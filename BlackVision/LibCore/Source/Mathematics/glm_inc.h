#pragma once

// Disable warning C4201: nonstandard extension used : nameless struct/union
#pragma warning(push)
#pragma warning(disable : 4201)

#define GLM_FORCE_SSE2
#define GLM_FORCE_ALIGNED

#include "glm.hpp"

#include "gtx/quaternion.hpp"
#include "gtc/matrix_transform.hpp"

#include "gtx/euler_angles.hpp"
#include "gtx/vector_angle.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/matrix_transform_2d.hpp"

//#include "gtx/simd_mat4.hpp"

#pragma warning(pop)
