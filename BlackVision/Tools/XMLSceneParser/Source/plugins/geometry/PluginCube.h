#pragma once

#include "../base/XMLPlugin.h"

using namespace std;

class PluginCube:public XMLPlugin
{
public:
	PluginCube()
	{
		properties.push_back(Property(Bevel,"bevel"));
		non_linear_properties.push_back(NonLinearProperty(Tesselation,"tessalation"));
		name="cube";
		type=PLUGIN_TYPE_GEOMETRY;
	}
	vector<TimeProperty> Bevel;

	string Tesselation;
    void DefaultPluginProperties()
    {
		Bevel.push_back(TimeProperty("0","0.4"));
		Tesselation="10";
	}
	
};