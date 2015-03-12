#pragma once

#include <string>
#include "CoreDEF.h"
#include "Memory/MemoryChunk.h"

namespace bv { namespace model
{

class Asset;
DEFINE_CONST_PTR_TYPE( Asset )

class Asset
{
protected:
	virtual VoidConstPtr					QueryThis	() const = 0;

public:

	virtual const std::string &				GetUID		() const = 0;

    virtual ~Asset(){}

	template< typename AssetTypeConstPtr >
	friend AssetTypeConstPtr  QueryTypedRes( AssetTypeConstPtr res );
};

template< typename AssetTypeConstPtr >
AssetTypeConstPtr  QueryTypedRes( AssetConstPtr res )
{
	if( res->GetUID() != AssetTypeConstPtr::element_type::UID() )
    {
        return nullptr;
    }

    return std::static_pointer_cast< AssetTypeConstPtr::element_type >( res->QueryThis() );
}


} // model
} // bv
