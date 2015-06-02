#pragma once
#include "../base/XMLPlugin.h"

using namespace std;

class PluginInstance: public XMLPlugin
{
public:
	PluginInstance()  
	{
		name="instance";
		type=PLUGIN_TYPE_FUNCTION;
        non_linear_properties.push_back(NonLinearProperty(class_name,"class"));
	}
	string                  class_name;
};