#include "BVGLPluginFactory.h"

#include "BVGLPlugins/BVGLPlugin.h"
#include "BVGLPlugins/BVGLResourceTrackingPlugin.h"

namespace bv {

namespace {

BVGLPlugin					defaultPlugin;
BVGLResourceTrackingPlugin	resourceTrackingPlugin;

}

// *****************************
//
BVGLPlugin &		BVGLPluginFactory::BVGL	()
{
	return BVGL( BVGLPluginType::BVGL_RESOURCE_TRACKER );
}

// *****************************
//
BVGLPlugin &		BVGLPluginFactory::BVGL	( BVGLPluginType bvglType )
{
	switch( bvglType )
	{
		case BVGLPluginType::BVGL_RESOURCE_TRACKER:
			return resourceTrackingPlugin;
		default:
			return defaultPlugin;
	}
}

} //bv
