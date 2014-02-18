#pragma once

#include <string>

#include "Engine/Models/Plugins/Interfaces/IValueSet.h"


namespace bv { namespace model
{

class IShaderChannel : public IValueSet
{
public:

    virtual const std::string &     GetShaderSource ()  const = 0;
    virtual bool                    IsReadOnly      ()  const = 0;  //Should be in IChannel
    virtual void                    PostUpdate      () = 0;         //Should also be in IChannel

    virtual                         ~IShaderChannel () {}

};

} // model
} // bv
