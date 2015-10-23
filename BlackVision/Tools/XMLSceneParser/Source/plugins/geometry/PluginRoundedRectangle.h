#pragma once

#include "../base/XMLPlugin.h"

using namespace std;

class PluginRoundedRectangle:public XMLPlugin
{
public:
	PluginRoundedRectangle()
	{
		properties.push_back(Property(Bevels,"bevels"));
        properties.push_back(Property(Size,"size"));
		name="rounded_rectangle";
		type=PLUGIN_TYPE_GEOMETRY;
	}
    void DefaultPluginProperties()
    {
		Size.push_back(TimeProperty("0","800,200"));
		Bevels.push_back(TimeProperty("0","0.1,0.1,0.1,0.1"));
    }
	vector<TimeProperty> Bevels;
	vector<TimeProperty> Size;
};