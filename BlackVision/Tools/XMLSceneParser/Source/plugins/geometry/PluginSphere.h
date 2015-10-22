#pragma once

#include "../base/XMLPlugin.h"

using namespace std;

class PluginSphere:public XMLPlugin
{
public:
	PluginSphere()
	{
		properties.push_back(Property(VerticalStripes,"vertical_stripes"));
		properties.push_back(Property(HorizontalStripes,"horizontal_stripes"));
        properties.push_back(Property(OpenAngle,"open_angle"));
		non_linear_properties.push_back(NonLinearProperty(OpenAngleMode,"open_angle_mode"));
		non_linear_properties.push_back(NonLinearProperty(Radius,"radius"));
		name="circle";
		type=PLUGIN_TYPE_GEOMETRY;
	}
    void DefaultPluginProperties()
    {
		VerticalStripes.push_back(TimeProperty("0","1"));
		HorizontalStripes.push_back(TimeProperty("0","0"));
		OpenAngle.push_back(TimeProperty("0","0"));
		OpenAngleMode="CW";
		Radius="1";
    }
	vector<TimeProperty> VerticalStripes;
	vector<TimeProperty> HorizontalStripes;
	vector<TimeProperty> OpenAngle;
	string OpenAngleMode;
	string Radius;
};