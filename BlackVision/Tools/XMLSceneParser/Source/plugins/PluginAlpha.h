#pragma once
#include "XMLPlugin.h"

using namespace std;


class PluginAlpha:public XMLPlugin
{
public:
	PluginAlpha()
	{

		properties.push_back(Property(alpha,"alpha"));
		

		name="alpha";
		type=PLUGIN_TYPE_GEOMETRY;
	}

	vector<TimeProperty> alpha;
	void DefaultPluginProperties()
    {
        alpha.push_back(TimeProperty("0","1"));
    }
};
