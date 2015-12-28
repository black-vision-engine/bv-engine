#pragma once

#include "../base/XMLPlugin.h"

using namespace std;

class PluginVideoInput:public XMLPlugin
{
public:
	PluginVideoInput()
	{
		Source="1";
		non_linear_properties.push_back(NonLinearProperty(Source,"source"));
		name="video_input";
		type=PLUGIN_TYPE_PIXEL_SHADER;

	}

	string Source;
	


};