#pragma once
#include "../base/XMLPlugin.h"

using namespace std;

class PluginCrawl : public XMLPlugin
{
public:
	PluginCrawl()  
	{
		name="crawl";
		type=PLUGIN_TYPE_FUNCTION;
		text_offset="0,0,0";

        non_linear_properties.push_back(NonLinearProperty(speed,"speed"));
        non_linear_properties.push_back(NonLinearProperty(interspace,"interspace"));
        non_linear_properties.push_back(NonLinearProperty(count,"count"));
		non_linear_properties.push_back(NonLinearProperty(autostart,"autostart"));
		non_linear_properties.push_back(NonLinearProperty(font_name,"font"));
		non_linear_properties.push_back(NonLinearProperty(size,"size"));
		non_linear_properties.push_back(NonLinearProperty(text_offset,"text_offset"));
		non_linear_properties.push_back(NonLinearProperty(text_color,"text_color"));
		non_linear_properties.push_back(NonLinearProperty(separator_image,"separator_image"));
		non_linear_properties.push_back(NonLinearProperty(promo_msg,"promo_msg"));
		non_linear_properties.push_back(NonLinearProperty(promo_freq,"promo_freq"));
	}

    string					promo_freq;
	wstring					promo_msg;
	string                  size;
	string                  text_offset;
	string                  text_color;
	string                  speed;
    string                  interspace;
    string                  count;
	string                  autostart;
	string                  font_name;
	string                  separator_image;
};