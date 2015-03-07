#pragma once

#include "ResourceDescriptor.h"
#include "IResource.h"

namespace bv { namespace model {

class ResourceLoader
{
public:

	virtual IResourceConstPtr		LoadResource        ( const ResourceDescConstPtr & desc )  const = 0;

            ResourceLoader		();
    virtual ~ResourceLoader		();

};

} // model
} // bv
