#pragma once
#include "../base/XMLPlugin.h"

using namespace std;
class PluginText:public XMLPlugin
{
public:
	PluginText()
	{
		Text=L"";
		Size="12";
		OutlineSize="0";
		BlurSize="0";
		Font="";
        max_size="null";
		OutlineColor_RGBA="1,1,1,1";
		non_linear_properties.push_back(NonLinearProperty(Text,"text"));
		non_linear_properties.push_back(NonLinearProperty(OutlineSize,"outline_size"));
		non_linear_properties.push_back(NonLinearProperty(OutlineColor_RGBA,"outline_color"));
		non_linear_properties.push_back(NonLinearProperty(BlurSize,"blur_size"));
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
	string OutlineColor_RGBA;
	string OutlineSize;
	string BlurSize;
	string Font;
    string Align;
	
};