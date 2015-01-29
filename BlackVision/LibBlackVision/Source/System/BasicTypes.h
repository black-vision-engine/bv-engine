#pragma once

#include <memory>

#define DEFINE_PTR_TYPE(Type) typedef std::shared_ptr< Type > Type##Ptr;
#define DEFINE_CONST_PTR_TYPE(Type) typedef std::shared_ptr< const Type > Type##ConstPtr;

typedef std::shared_ptr< void > VoidPtr;
typedef std::shared_ptr< const void > VoidConstPtr;

namespace bv {

typedef float				Float32;
typedef double				Float64;

typedef signed char			Int8;
typedef signed short		Int16;
typedef signed long long	Int64;
typedef signed int			Int32;

typedef unsigned char		UInt8;
typedef unsigned short		UInt16;
typedef unsigned long long	UInt64;
typedef unsigned int		UInt32;
typedef size_t				SizeType;

typedef Float32				TimeType;

} //bv
