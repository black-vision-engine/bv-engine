#pragma once

#include "Engine/Interfaces/IValueSet.h"

namespace bv {

class IShaderDataSource : public IValueSet
{
public:

    virtual const std::string &     GetShaderSource     ()  const = 0;

    virtual                         ~IShaderDataSource  () {}
};

} //bv
