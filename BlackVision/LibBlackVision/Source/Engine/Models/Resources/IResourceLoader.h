#pragma once

#include "Engine/Models/Resources/IResource.h"
#include "ResourceDescriptor.h"

namespace bv { namespace model {

class IResourceLoader
{
public:
	virtual IResourceNEWConstPtr    LoadResource        ( const ResourceDescConstPtr & desc )  const   { return nullptr; } //FIXME: MAKE THIS FUNCTION PURE VIRTUAL;
    virtual ResourceHandle *        LoadResource        ( IResource* res )  const    = 0; // DEPRICATED

    virtual ~IResourceLoader(){}
};

} // model
} // bv
