#pragma once
#include "XMLPlugin.h"

using namespace std;


class PluginRing:public XMLPlugin
{
public:
	PluginRing()
	{
		inner_radius="1";
		outer_radius="1";
		start_angle="0";
		end_angle="3.14";
		segment_count="10";
		
		non_linear_properties.push_back(NonLinearProperty(inner_radius,"inner_radius"));
		non_linear_properties.push_back(NonLinearProperty(outer_radius,"outer_radius"));
		non_linear_properties.push_back(NonLinearProperty(start_angle,"start_angle"));
		non_linear_properties.push_back(NonLinearProperty(end_angle,"end_angle"));
		non_linear_properties.push_back(NonLinearProperty(segment_count,"segment_count"));
		
		name="ring";
		type=PLUGIN_TYPE_GEOMETRY;
	}

	string inner_radius;
	string outer_radius;
	string start_angle;
	string end_angle;
	string segment_count;
};
