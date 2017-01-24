#pragma once

#include "Enums.h"

#include <type_traits>

namespace bv
{

template< ModelParamType MPT >
struct ModelParamType2ParamType
{
};

template<>
struct ModelParamType2ParamType< ModelParamType::MPT_FLOAT >
{
	static const ParamType PT = ParamType::PT_FLOAT1;
};

template<>
struct ModelParamType2ParamType< ModelParamType::MPT_BOOL >
{
	static const ParamType PT = ParamType::PT_BOOL;
};

template<>
struct ModelParamType2ParamType< ModelParamType::MPT_ENUM >
{
	static const ParamType PT = ParamType::PT_ENUM;
};

template<>
struct ModelParamType2ParamType< ModelParamType::MPT_INT >
{
	static const ParamType PT = ParamType::PT_INT;
};

template<>
struct ModelParamType2ParamType< ModelParamType::MPT_MAT2 >
{
	static const ParamType PT = ParamType::PT_MAT2;
};

template<>
struct ModelParamType2ParamType< ModelParamType::MPT_STRING >
{
	static const ParamType PT = ParamType::PT_STRING;
};

template<>
struct ModelParamType2ParamType< ModelParamType::MPT_VEC2 >
{
	static const ParamType PT = ParamType::PT_FLOAT2;
};

template<>
struct ModelParamType2ParamType< ModelParamType::MPT_VEC3 >
{
	static const ParamType PT = ParamType::PT_FLOAT3;
};

template<>
struct ModelParamType2ParamType< ModelParamType::MPT_VEC4 >
{
	static const ParamType PT = ParamType::PT_FLOAT4;
};

template<>
struct ModelParamType2ParamType< ModelParamType::MPT_WSTRING >
{
	static const ParamType PT = ParamType::PT_WSTRING;
};



template< ParamType PT >
struct ParamType2ModelParamType
{};

template<>
struct ParamType2ModelParamType< ParamType::PT_FLOAT1 >
{
	static const ModelParamType MPT = ModelParamType::MPT_FLOAT;
};

template<>
struct ParamType2ModelParamType< ParamType::PT_BOOL >
{
	static const ModelParamType MPT = ModelParamType::MPT_BOOL;
};

template<>
struct ParamType2ModelParamType< ParamType::PT_ENUM >
{
	static const ModelParamType MPT = ModelParamType::MPT_ENUM;
};

template<>
struct ParamType2ModelParamType< ParamType::PT_INT >
{
	static const ModelParamType MPT = ModelParamType::MPT_INT;
};

template<>
struct ParamType2ModelParamType< ParamType::PT_MAT2 >
{
	static const ModelParamType MPT = ModelParamType::MPT_MAT2;
};

template<>
struct ParamType2ModelParamType< ParamType::PT_STRING >
{
	static const ModelParamType MPT = ModelParamType::MPT_STRING;
};

template<>
struct ParamType2ModelParamType< ParamType::PT_FLOAT2 >
{
	static const ModelParamType MPT = ModelParamType::MPT_VEC2;
};

template<>
struct ParamType2ModelParamType< ParamType::PT_FLOAT3 >
{
	static const ModelParamType MPT = ModelParamType::MPT_VEC3;
};

template<>
struct ParamType2ModelParamType< ParamType::PT_FLOAT4 >
{
	static const ModelParamType MPT = ModelParamType::MPT_VEC4;
};

template<>
struct ParamType2ModelParamType< ParamType::PT_WSTRING >
{
	static const ModelParamType MPT = ModelParamType::MPT_WSTRING;
};

template< typename T, typename = void >
struct Type2InterpolatorType
{
	typedef T Type;
};

template< typename T >
struct Type2InterpolatorType< T, typename std::enable_if< std::is_enum< T >::value >::type >
{
	typedef Int32 Type;
};


};