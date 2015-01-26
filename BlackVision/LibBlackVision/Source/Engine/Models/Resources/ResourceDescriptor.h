#pragma once

#include "System/BasicTypes.h"

#include <string>

namespace bv
{

class ResourceDesc;
DEFINE_CONST_PTR_TYPE( ResourceDesc )

class ResourceDesc
{
protected:
	virtual const std::string &		UID			() const = 0;

public:
	bool							Query		( const std::string & uid ) const;
	virtual bool					IsCacheable	() const = 0;
};

} // bv