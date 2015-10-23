#pragma once

#include "../base/XMLPlugin.h"

using namespace std;

class PluginTriangle:public XMLPlugin
{
public:
	PluginTriangle()
	{
		properties.push_back(Property(PointA,"point_a"));
        properties.push_back(Property(PointB,"point_b"));
		properties.push_back(Property(PointC,"point_c"));
		name="triangle";
		type=PLUGIN_TYPE_GEOMETRY;
	}
    void DefaultPluginProperties()
    {
		PointA.push_back(TimeProperty("0","-1,0,0"));
		PointB.push_back(TimeProperty("0","1,0,0"));
		PointC.push_back(TimeProperty("0","0,1,0"));
    }
	vector<TimeProperty> PointA;
	vector<TimeProperty> PointB;
	vector<TimeProperty> PointC;
};