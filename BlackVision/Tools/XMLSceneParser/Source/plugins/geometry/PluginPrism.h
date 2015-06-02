#pragma once

#include "../base/XMLPlugin.h"

using namespace std;

class PluginPrism:public XMLPlugin
{
public:
	PluginPrism()
	{
		vertices_number="4";
		
		non_linear_properties.push_back(NonLinearProperty(vertices_number,"vertices_number"));
		name="prism";
		type=PLUGIN_TYPE_GEOMETRY;
	}
    void DefaultPluginProperties()
    {
        vertices_number="4";
    }
	string vertices_number;

};