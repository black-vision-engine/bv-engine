#pragma once

#include <memory>

#define DEFINE_PTR_TYPE(Type) typedef std::shared_ptr< Type > Type##Ptr;

typedef std::shared_ptr< void > VoidPtr;

namespace bv {

typedef float TimeType;

} //bv
