#pragma once

#include <memory>
#include <vector>

#include "BaseValue.h"
#include "Mathematics/glm_inc.h"


namespace bv {

typedef ValueImpl< int, ParamType::PT_INT >             ValueInt;
typedef ValueImpl< float, ParamType::PT_FLOAT1 >        ValueFloat;
typedef ValueImpl< glm::vec2, ParamType::PT_FLOAT2 >    ValueVec2;
typedef ValueImpl< glm::vec3, ParamType::PT_FLOAT3 >    ValueVec3;
typedef ValueImpl< glm::vec4, ParamType::PT_FLOAT4 >    ValueVec4;
typedef ValueImpl< glm::mat2, ParamType::PT_MAT2 >      ValueMat2;
typedef ValueImpl< glm::mat2, ParamType::PT_MAT3 >      ValueMat3;
typedef ValueImpl< glm::mat4, ParamType::PT_MAT4 >      ValueMat4;
typedef ValueImpl< std::string, ParamType::PT_STRING >  ValueString;
typedef ValueImpl< std::wstring, ParamType::PT_WSTRING > ValueWString;

DEFINE_PTR_TYPE(ValueInt)
DEFINE_PTR_TYPE(ValueFloat)
typedef std::shared_ptr< const ValueFloat >   ValueFloatConstPtr;
DEFINE_PTR_TYPE(ValueVec2)
DEFINE_PTR_TYPE(ValueVec3)
DEFINE_PTR_TYPE(ValueVec4)
DEFINE_PTR_TYPE(ValueMat2)
DEFINE_PTR_TYPE(ValueMat3)
DEFINE_PTR_TYPE(ValueMat4)
DEFINE_PTR_TYPE(ValueString)
DEFINE_PTR_TYPE(ValueWString)

typedef std::vector< ValueMat4Ptr >                 ValueMat4PtrVec;
typedef std::shared_ptr< const ValueMat4PtrVec >    ValueMat4PtrVecConstPtr;
DEFINE_PTR_TYPE(ValueMat4PtrVec)

} //bv
