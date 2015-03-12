#pragma once

#include "CoreDEF.h"

#include <string>

namespace bv
{

class ResourceDesc;
DEFINE_CONST_PTR_TYPE( ResourceDesc )

class ResourceDesc
{
protected:
	virtual VoidConstPtr            QueryThis	() const = 0;

public:
	virtual const std::string &		GetUID		() const = 0;

	virtual bool					IsCacheable	() const = 0;


	template< typename DescTypeConstPtr >
	friend DescTypeConstPtr  QueryTypedDesc( ResourceDescConstPtr desc );
};

// *********************************
//
template< typename DescTypeConstPtr >
DescTypeConstPtr  QueryTypedDesc( ResourceDescConstPtr desc )
{
    if( desc->GetUID() != DescTypeConstPtr::element_type::UID() )
    {
        return nullptr;
    }

    return std::static_pointer_cast< DescTypeConstPtr::element_type >( desc->QueryThis() );
}

} // bv