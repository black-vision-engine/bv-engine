#pragma once

#include <memory>
#include <vector>
#include <type_traits>

#include "BaseValue.h"
#include "Mathematics/glm_inc.h"


namespace bv {

template< typename ValueType, typename = void >
struct Value2ParamTypeTrait
{ };

template< typename ValueType >
struct Value2ParamTypeTrait< ValueType, typename std::enable_if< std::is_enum< ValueType >::value >::type >
{ const static ParamType ParamT = ParamType::PT_ENUM; };

template<>
struct Value2ParamTypeTrait< bool >
{ const static ParamType ParamT = ParamType::PT_BOOL; };

template<>
struct Value2ParamTypeTrait< Int32 >
{ const static ParamType ParamT = ParamType::PT_INT; };

template<>
struct Value2ParamTypeTrait< Float32 >
{ const static ParamType ParamT = ParamType::PT_FLOAT1; };

template<>
struct Value2ParamTypeTrait< Float64 >
{ const static ParamType ParamT = ParamType::PT_DOUBLE1; };

template<>
struct Value2ParamTypeTrait< glm::vec2 >
{ const static ParamType ParamT = ParamType::PT_FLOAT2; };

template<>
struct Value2ParamTypeTrait< glm::vec3 >
{ const static ParamType ParamT = ParamType::PT_FLOAT3; };

template<>
struct Value2ParamTypeTrait< glm::vec4 >
{ const static ParamType ParamT = ParamType::PT_FLOAT4; };

template<>
struct Value2ParamTypeTrait< glm::mat2 >
{ const static ParamType ParamT = ParamType::PT_MAT2; };

template<>
struct Value2ParamTypeTrait< glm::mat3 >
{ const static ParamType ParamT = ParamType::PT_MAT3; };

template<>
struct Value2ParamTypeTrait< glm::mat4 >
{ const static ParamType ParamT = ParamType::PT_MAT4; };

template<>
struct Value2ParamTypeTrait< std::string >
{ const static ParamType ParamT = ParamType::PT_STRING; };

template<>
struct Value2ParamTypeTrait< std::wstring >
{ const static ParamType ParamT = ParamType::PT_WSTRING; };

template< typename ValueType >
struct ValueT
{
    typedef ValueImpl< ValueType, Value2ParamTypeTrait< ValueType >::ParamT > Type;
};

typedef ValueT< bool >::Type            ValueBool;
typedef ValueT< Int32 >::Type           ValueInt;
typedef ValueT< Float32 >::Type         ValueFloat;
typedef ValueT< Float64 >::Type         ValueDouble;
typedef ValueT< glm::vec2 >::Type       ValueVec2;
typedef ValueT< glm::vec3 >::Type       ValueVec3;
typedef ValueT< glm::vec4 >::Type       ValueVec4;
typedef ValueT< glm::mat2 >::Type       ValueMat2;
typedef ValueT< glm::mat3 >::Type       ValueMat3;
typedef ValueT< glm::mat4 >::Type       ValueMat4;
typedef ValueT< std::string >::Type     ValueString;
typedef ValueT< std::wstring >::Type    ValueWString;

DEFINE_PTR_TYPE( ValueBool )
DEFINE_PTR_TYPE( ValueInt )
DEFINE_PTR_TYPE( ValueFloat )
DEFINE_PTR_TYPE( ValueDouble )
typedef std::shared_ptr< const ValueFloat >   ValueFloatConstPtr;
DEFINE_PTR_TYPE( ValueVec2 )
DEFINE_PTR_TYPE( ValueVec3 )
DEFINE_PTR_TYPE( ValueVec4 )
DEFINE_PTR_TYPE( ValueMat2 )
DEFINE_PTR_TYPE( ValueMat3 )
DEFINE_PTR_TYPE( ValueMat4 )
DEFINE_PTR_TYPE( ValueString )
DEFINE_PTR_TYPE( ValueWString )

} //bv
