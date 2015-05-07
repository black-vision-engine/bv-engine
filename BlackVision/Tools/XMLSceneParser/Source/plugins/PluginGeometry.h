#pragma once

#include <list>
#include <vector>
#include<string>

using namespace std;



class PluginGeometry:public XMLPlugin
{
public:
	PluginGeometry()
	{
		properties.push_back(Property(position,"position"));
		properties.push_back(Property(rotation,"rotation"));
		properties.push_back(Property(scaling,"scaling"));
        properties.push_back(Property(weight,"weight"));
		name="geometry";
		type=PLUGIN_TYPE_TRANSFORM;
	}

    void DefaultPluginProperties()
    {
        position.push_back(TimeProperty("0","0,0,0"));
        //weight.push_back(TimeProperty("0","0,0,0"));
        rotation.push_back(TimeProperty("0","0,0,1,0"));
        scaling.push_back(TimeProperty("0","1,1,1"));
    }

	vector<TimeProperty> weight;
    vector<TimeProperty> position;
	vector<TimeProperty> rotation;
    vector<TimeProperty> scaling;

};


