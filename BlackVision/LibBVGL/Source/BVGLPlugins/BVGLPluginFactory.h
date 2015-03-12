#pragma once

#include "BVGLPlugins/BVGLPlugin.h"

namespace bv {


enum class BVGLPluginType : int
{
	BVGL_DEFAULT = 0,
	BVGL_RESOURCE_TRACKER,
	BVGL_TOTAL
};

class BVGLPluginFactory
{
public:

	static BVGLPlugin &		BVGL();
	static BVGLPlugin &		BVGL( BVGLPluginType bvglType );

};

} //bv
