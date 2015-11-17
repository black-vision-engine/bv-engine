#pragma once

#include <string>
#include "CoreDEF.h"
#include "Assets/FwdDecls.h"
#include "Memory/MemoryChunk.h"

namespace bv {

class Asset
{
protected:
	virtual VoidConstPtr					QueryThis	() const = 0;

public:

	virtual const std::string &				GetUID		() const = 0;

    virtual ~Asset(){}

	template< typename AssetTypeConstPtr >
	friend AssetTypeConstPtr  QueryTypedRes( AssetTypeConstPtr asset );
};

// ****************************
//
template< typename AssetTypeConstPtr >
AssetTypeConstPtr  QueryTypedRes( AssetConstPtr asset )
{
	if( asset->GetUID() != AssetTypeConstPtr::element_type::UID() )
    {
        return nullptr;
    }

    return std::static_pointer_cast< AssetTypeConstPtr::element_type >( asset->QueryThis() );
}




} // bv
