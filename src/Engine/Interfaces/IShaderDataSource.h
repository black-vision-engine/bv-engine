#pragma once

#include "Engine/Interfaces/IValueSet.h"
#include "Engine/Models/Interfaces/ITexturesData.h"


namespace bv {

class IShaderDataSource : public IValueSet
{
public:

    virtual const std::string &     GetShaderSource     ()  const = 0;
    virtual ITexturesDataConstPtr   GetTexturesData     ()  const = 0;

    virtual                         ~IShaderDataSource  () {}
};

} //bv
