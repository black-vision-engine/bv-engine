#pragma once

#include "../base/XMLPlugin.h"

using namespace std;

class PluginTexture:public XMLPlugin
{
public:
	PluginTexture()
	{
		Path="";
        properties.push_back(Property(alpha,"alpha"));
		non_linear_properties.push_back(NonLinearProperty(Path,"path"));
		name="texture";
		type=PLUGIN_TYPE_PIXEL_SHADER;

	}

	string Path;
	vector<TimeProperty> alpha;


};