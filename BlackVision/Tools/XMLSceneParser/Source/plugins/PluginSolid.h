#pragma once
#include "XMLPlugin.h"

using namespace std;

class PluginSolid:public XMLPlugin
{
public:
	PluginSolid()
	{
		properties.push_back(Property(RGBA,"RGBA"));
		properties.push_back(Property(R,"R"));
		properties.push_back(Property(G,"G"));
		properties.push_back(Property(B,"B"));
		properties.push_back(Property(A,"A"));
		name="solid";
		type=PLUGIN_TYPE_COLOR;

	}
    string surface;
    vector<TimeProperty> RGBA;
	vector<TimeProperty> R;
	vector<TimeProperty> G;
	vector<TimeProperty> B;
	vector<TimeProperty> A;
    void DefaultPluginProperties()
    {
        RGBA.push_back(TimeProperty("0","0,0,0,1"));
    }
	
};