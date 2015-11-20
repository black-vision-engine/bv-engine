#pragma once

#include "../base/XMLPlugin.h"

using namespace std;

class PluginSequence:public XMLPlugin
{
public:
	PluginSequence()
	{
		Path="";
		Extension="tga";
		File="";
		FPS="25";
		FrameCount="0";
        FrameBegin="0";
		non_linear_properties.push_back(NonLinearProperty(Extension,"extension"));
		non_linear_properties.push_back(NonLinearProperty(File,"file"));
		non_linear_properties.push_back(NonLinearProperty(FPS,"fps"));
		non_linear_properties.push_back(NonLinearProperty(Path,"path"));
		non_linear_properties.push_back(NonLinearProperty(FrameCount,"frame_count"));
        non_linear_properties.push_back(NonLinearProperty(FrameBegin,"frame_begin"));
        properties.push_back(Property(frames_maping,"frames_maping"));
		name="sequence";
		type=PLUGIN_TYPE_PIXEL_SHADER;
	}
    vector<TimeProperty> frames_maping;
	string Path;
	string FrameCount;
    string FrameBegin;
	string Extension;
	string File;
	string FPS;
};