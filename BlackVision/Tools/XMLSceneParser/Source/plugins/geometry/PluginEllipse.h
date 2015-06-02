#pragma once

#include "../base/XMLPlugin.h"

using namespace std;

class PluginEllipse:public XMLPlugin
{
public:
	vector<TimeProperty> OuterRadius1;
	vector<TimeProperty> OuterRadius2;
	string OpenAngleMode;
	string Tesselation;

	PluginEllipse()
	{
		properties.push_back(Property(OuterRadius1,"outer_radius1"));
		properties.push_back(Property(OuterRadius2,"outer_radius2"));
		non_linear_properties.push_back(NonLinearProperty(Tesselation,"tessalation"));
		name="ellipse";
		type=PLUGIN_TYPE_GEOMETRY;
	}
    void DefaultPluginProperties()
    {
		OuterRadius1.push_back(TimeProperty("0","1"));
		OuterRadius2.push_back(TimeProperty("0","2"));
		OpenAngleMode="CW";
		Tesselation="10";
    }

};