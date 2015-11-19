#pragma once
#include "../base/XMLPlugin.h"

using namespace std;

class PluginCounter : public XMLPlugin
{
public:
	PluginCounter()  
	{
		name="counter";
		type=PLUGIN_TYPE_FUNCTION;
		
        non_linear_properties.push_back(NonLinearProperty(precision,"precision"));
		properties.push_back(Property(values,"values"));
		
	}

	void DefaultPluginProperties()
    {
        values.push_back(TimeProperty("0","0"));
    }

	vector<TimeProperty>	values;

    string					precision;

	
	
};