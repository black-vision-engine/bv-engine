#pragma once

#include <memory>
#include <vector>

#include "BaseValue.h"
#include "glm/glm.hpp"


namespace bv {

typedef model::ValueImpl< float, ParamType::PT_FLOAT >         ValueFloat;
typedef model::ValueImpl< glm::vec3, ParamType::PT_FLOAT3 >    ValueVec3;
typedef model::ValueImpl< glm::vec4, ParamType::PT_FLOAT4 >    ValueVec4;
typedef model::ValueImpl< glm::mat2, ParamType::PT_MAT2 >      ValueMat2;
typedef model::ValueImpl< glm::mat4, ParamType::PT_MAT4 >      ValueMat4;

typedef std::shared_ptr< ValueFloat >   ValueFloatPtr;
typedef std::shared_ptr< ValueVec3 >    ValueVec3Ptr;
typedef std::shared_ptr< ValueVec4 >    ValueVec4Ptr;
typedef std::shared_ptr< ValueMat2 >    ValueMat2Ptr;
typedef std::shared_ptr< ValueMat4 >    ValueMat4Ptr;

typedef std::vector< ValueMat4 * >  ValueMat4PtrVec;

} //bv
