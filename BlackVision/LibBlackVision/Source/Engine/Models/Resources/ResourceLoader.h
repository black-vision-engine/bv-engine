#pragma once

#include "ResourceDescriptor.h"
#include "Resource.h"

namespace bv { namespace model {

class ResourceLoader
{
public:

	virtual ResourceConstPtr		LoadResource        ( const ResourceDescConstPtr & desc )  const = 0;

            ResourceLoader		();
    virtual ~ResourceLoader		();

};

} // model
} // bv
