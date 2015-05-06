#pragma once
#include "XMLPlugin.h"

using namespace std;


class PluginMask:public XMLPlugin
{
public:
	PluginMask()
	{

		properties.push_back(Property(position,"position"));
		properties.push_back(Property(rotation,"rotation"));
		properties.push_back(Property(scaling,"scaling"));
		non_linear_properties.push_back(NonLinearProperty(MaskFilePath,"mask_file_path"));

		name="mask";
		type=PLUGIN_TYPE_MASK;
	}


    string MaskFilePath;
    vector<TimeProperty> position;
	vector<TimeProperty> rotation;
    vector<TimeProperty> scaling;
	void DefaultPluginProperties()
    {
       
        position.push_back(TimeProperty("0","0,0,0"));
        rotation.push_back(TimeProperty("0","0,0,0"));
        scaling.push_back(TimeProperty("0","1,1,1"));
    }
};
