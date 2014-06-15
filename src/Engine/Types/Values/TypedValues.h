#pragma once

#include <memory>
#include <vector>

#include "BaseValue.h"
#include "glm/glm.hpp"


namespace bv {

typedef ValueImpl< float, ParamType::PT_FLOAT1 >        ValueFloat;
typedef ValueImpl< glm::vec2, ParamType::PT_FLOAT2 >    ValueVec2;
typedef ValueImpl< glm::vec3, ParamType::PT_FLOAT3 >    ValueVec3;
typedef ValueImpl< glm::vec4, ParamType::PT_FLOAT4 >    ValueVec4;
typedef ValueImpl< glm::mat2, ParamType::PT_MAT2 >      ValueMat2;
typedef ValueImpl< glm::mat2, ParamType::PT_MAT3 >      ValueMat3;
typedef ValueImpl< glm::mat4, ParamType::PT_MAT4 >      ValueMat4;

typedef std::shared_ptr< ValueFloat >   ValueFloatPtr;
typedef std::shared_ptr< const ValueFloat >   ValueFloatConstPtr;
typedef std::shared_ptr< ValueVec2 >    ValueVec2Ptr;
typedef std::shared_ptr< ValueVec3 >    ValueVec3Ptr;
typedef std::shared_ptr< ValueVec4 >    ValueVec4Ptr;
typedef std::shared_ptr< ValueMat2 >    ValueMat2Ptr;
typedef std::shared_ptr< ValueMat3 >    ValueMat3Ptr;
typedef std::shared_ptr< ValueMat4 >    ValueMat4Ptr;

typedef std::vector< ValueMat4Ptr >                 ValueMat4PtrVec;
typedef std::shared_ptr< const ValueMat4PtrVec >    ValueMat4PtrVecConstPtr;
typedef std::shared_ptr< ValueMat4PtrVec >          ValueMat4PtrVecPtr;

} //bv
