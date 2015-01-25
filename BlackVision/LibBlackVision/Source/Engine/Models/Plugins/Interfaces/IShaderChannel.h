#pragma once

#include <string>

#include "Engine/Interfaces/IShaderDataSource.h"


namespace bv { namespace model
{

class IShaderChannel : public IShaderDataSource
{
public:

    virtual bool                    IsReadOnly      ()  const = 0;  //Should be in IChannel
    virtual void                    PostUpdate      () = 0;         //Should also be in IChannel

    virtual                         ~IShaderChannel () {}

};

DEFINE_PTR_TYPE(IShaderChannel)
DEFINE_CONST_PTR_TYPE(IShaderChannel)

} // model
} // bv
