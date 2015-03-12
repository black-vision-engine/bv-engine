#pragma once

#include "AssetDescriptor.h"
#include "Asset.h"

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
