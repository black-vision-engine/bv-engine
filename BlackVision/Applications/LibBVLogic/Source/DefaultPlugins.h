#pragma once

#include <vector>


namespace bv {
namespace model
{

class IPluginDescriptor;

std::vector< IPluginDescriptor * >  DefaultBVPluginDescriptors  ();
std::vector< IPluginDescriptor * >  RemainingBVPluginDescriptors();
std::vector< IPluginDescriptor * >  FullBVPluginDescriptorsList ();

} //model
} //bv
