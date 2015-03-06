#pragma once

#include "Engine/Models/Resources/IResource.h"
#include "ResourceDescriptor.h"

namespace bv { namespace model {

class IResourceLoader
{
public:
	virtual IResourceConstPtr		LoadResource        ( const ResourceDescConstPtr & desc )  const = 0;

    virtual ~IResourceLoader(){}
};

} // model
} // bv
