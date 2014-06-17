#pragma once

#include <memory>

#define DEFINE_PTR_TYPE(Type) typedef std::shared_ptr< Type > Type##Ptr;
#define DEFINE_CONST_PTR_TYPE(Type) typedef std::shared_ptr< const Type > Type##ConstPtr;

typedef std::shared_ptr< void > VoidPtr;

namespace bv {

typedef float TimeType;

} //bv
