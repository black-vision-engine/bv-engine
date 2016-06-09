#pragma once

#include "BVALPlugins/BVALPlugin.h"

namespace bv {


enum class BVALPluginType : int
{
	BVAL_DEFAULT = 0,
	BVAL_TOTAL
};

class BVALPluginFactory
{
public:

	static BVALPlugin &		BVAL();
	static BVALPlugin &		BVAL( BVALPluginType bvalType );

};

} //bv
