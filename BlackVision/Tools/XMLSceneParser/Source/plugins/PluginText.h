#pragma once
#include "XMLPlugin.h"

using namespace std;
class PluginText:public XMLPlugin
{
public:
	PluginText()
	{
		Text=L"";
		Size="";
		Font="";
        max_size="null";
		non_linear_properties.push_back(NonLinearProperty(Text,"text"));
		non_linear_properties.push_back(NonLinearProperty(Size,"size"));
		non_linear_properties.push_back(NonLinearProperty(Font,"font"));
		non_linear_properties.push_back(NonLinearProperty(Align,"align"));
        non_linear_properties.push_back(NonLinearProperty(max_size,"max_size"));
		name="text";
		type=PLUGIN_TYPE_TEXT;
	}

	wstring Text;
    string max_size;
	string Size;
	string Font;
    string Align;
	
};