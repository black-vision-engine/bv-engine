#pragma once

#include <memory>

#include "BaseValue.h"
#include "glm/glm.hpp"

namespace bv { namespace model {

typedef ValueImpl< float, ParamType::PT_FLOAT >         ValueFloat;
typedef ValueImpl< glm::vec4, ParamType::PT_FLOAT4 >    ValueVec4;
typedef ValueImpl< glm::mat2, ParamType::PT_MAT2 >      ValueMat2;
typedef ValueImpl< glm::mat4, ParamType::PT_MAT4 >      ValueMat4;

typedef std::shared_ptr< ValueFloat >   ValueFloatPtr;
typedef std::shared_ptr< ValueMat4 >    ValueMat4Ptr;
typedef std::shared_ptr< ValueMat2 >    ValueMat2Ptr;
typedef std::shared_ptr< ValueVec4 >    ValueVec4Ptr;

} //model
} //bv
