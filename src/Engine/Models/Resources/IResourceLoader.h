#pragma once

#include "IResource.h"

namespace bv { namespace model {

class IResourceLoader
{
public:
    virtual ResourceHandle *        LoadResource        ( IResource* res )  const    = 0;

    virtual ~IResourceLoader(){}
};

} // model
} // bv