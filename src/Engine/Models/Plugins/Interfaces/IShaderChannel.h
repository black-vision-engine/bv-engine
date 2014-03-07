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

} // model
} // bv
