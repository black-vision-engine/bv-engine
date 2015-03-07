#pragma once

#include <string>
#include "System/BasicTypes.h"
#include "Core/MemoryChunk.h"

namespace bv { namespace model
{

class IResource;
DEFINE_CONST_PTR_TYPE( IResource )

class IResource
{
protected:
	virtual VoidConstPtr					QueryThis	() const = 0;

public:

	virtual const std::string &				GetUID		() const = 0;

    virtual ~IResource(){}

	template< typename ResourceTypeConstPtr >
	friend ResourceTypeConstPtr  QueryTypedRes( ResourceTypeConstPtr res );
};

template< typename ResourceTypeConstPtr >
ResourceTypeConstPtr  QueryTypedRes( IResourceConstPtr res )
{
	if( res->GetUID() != ResourceTypeConstPtr::element_type::UID() )
    {
        return nullptr;
    }

    return std::static_pointer_cast< ResourceTypeConstPtr::element_type >( res->QueryThis() );
}


} // model
} // bv
