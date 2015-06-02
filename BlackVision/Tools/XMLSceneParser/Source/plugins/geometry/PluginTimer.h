#pragma once
#include "../base/XMLPlugin.h"

using namespace std;
class PluginTimer:public XMLPlugin
{
public:
	PluginTimer()
	{
		Text=L"";
		Size="";
		Font="";
        ShowMS="false";
		non_linear_properties.push_back(NonLinearProperty(Text,"text"));
		non_linear_properties.push_back(NonLinearProperty(Size,"size"));
		non_linear_properties.push_back(NonLinearProperty(Font,"font"));
		non_linear_properties.push_back(NonLinearProperty(Align,"align"));
        non_linear_properties.push_back(NonLinearProperty(ShowMS,"show_ms"));
		name="timer";
		type=PLUGIN_TYPE_TEXT;
	}

	wstring Text;
	string Size;
	string Font;
    string Align;
    string ShowMS;
	
};