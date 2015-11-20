#pragma once

#include "../base/XMLPlugin.h"

using namespace std;

class PluginCircle:public XMLPlugin
{
public:
	PluginCircle()
	{
		properties.push_back(Property(OuterRadius,"outer_radius"));
		properties.push_back(Property(InnerRadius,"inner_radius"));
        properties.push_back(Property(OpenAngle,"open_angle"));
		non_linear_properties.push_back(NonLinearProperty(OpenAngleMode,"open_angle_mode"));
		non_linear_properties.push_back(NonLinearProperty(Tesselation,"tessalation"));
		name="circle";
		type=PLUGIN_TYPE_GEOMETRY;
	}
	vector<TimeProperty> OuterRadius;	
	vector<TimeProperty> InnerRadius;
	vector<TimeProperty> OpenAngle;
	string OpenAngleMode;
	string Tesselation;
    void DefaultPluginProperties()
    {
		OuterRadius.push_back(TimeProperty("0","1"));
		InnerRadius.push_back(TimeProperty("0","0"));
		OpenAngle.push_back(TimeProperty("0","0"));
		OpenAngleMode="CW";
		Tesselation="10";
    }
	
};