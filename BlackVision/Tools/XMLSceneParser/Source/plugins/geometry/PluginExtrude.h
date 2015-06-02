#pragma once

#include "../base/XMLPlugin.h"

using namespace std;
class PluginExtrude:public XMLPlugin
{
public:
	PluginExtrude()
	{
		
		properties.push_back(Property(depth,"depth"));
		name="extrude";
		type=PLUGIN_GEOMETRY_SHADER;

	}
	vector<TimeProperty> depth;

};