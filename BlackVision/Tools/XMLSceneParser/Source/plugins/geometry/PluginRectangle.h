#pragma once

#include "../base/XMLPlugin.h"

using namespace std;

class PluginRectangle:public XMLPlugin
{
public:
	PluginRectangle()
	{
		width="1";
		height="1";
        autosize="false";
		
		non_linear_properties.push_back(NonLinearProperty(width,"width"));
        non_linear_properties.push_back(NonLinearProperty(autosize,"autosize"));
		non_linear_properties.push_back(NonLinearProperty(height,"height"));
		name="rectangle";
		type=PLUGIN_TYPE_GEOMETRY;
	}
    void DefaultPluginProperties()
    {
        width="0";
        autosize="false";
        height="0";
    }
	string width;
	string height;
    string autosize;
};