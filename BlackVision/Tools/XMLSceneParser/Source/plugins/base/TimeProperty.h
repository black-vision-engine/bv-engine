#pragma once

#include <list>
#include <vector>
#include<string>
#include<conio.h>
using namespace std;

enum PLUGIN_TYPE{PLUGIN_TYPE_TRANSFORM,PLUGIN_TYPE_GEOMETRY,PLUGIN_TYPE_COLOR,PLUGIN_TYPE_TEXT,PLUGIN_VERTEX_SHADER,PLUGIN_GEOMETRY_SHADER,PLUGIN_TYPE_PIXEL_SHADER, PLUGIN_TYPE_MASK,PLUGIN_TYPE_FUNCTION};

class TimeProperty{
public:
	TimeProperty(){};
	TimeProperty(string _time,string _value){time=_time;value=_value;smoothing="0";};
    TimeProperty(string _time,string _value,string _smoothing){time=_time;value=_value;smoothing=_smoothing;};
	string time;
	string value;
    string smoothing;


};