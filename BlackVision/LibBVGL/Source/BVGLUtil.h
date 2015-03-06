#pragma once

#define SELECT_BVGL_PLUGIN(PluginName) \
namespace bv { \
    typedef PluginName BVGL; \
} //bv
