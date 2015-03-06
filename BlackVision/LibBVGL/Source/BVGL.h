#pragma once

#include "BVGLUtil.h"

#define BVGL_PLUGIN_TRACK_RESOURCES

#if defined(BVGL_PLUGIN_TRACK_RESOURCES)

    #include "BVGLPlugins/BVGLResourceTrackingPlugin.h"
    SELECT_BVGL_PLUGIN(BVGLResourceTrackingPlugin)

#else

    #include "BVGLPlugins/BVGLDefaultPlugin.h"
    SELECT_BVGL_PLUGIN(BVGLResourceTrackingPlugin)

#endif
