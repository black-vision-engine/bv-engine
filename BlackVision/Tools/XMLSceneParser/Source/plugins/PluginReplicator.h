#pragma once
#include "XMLPlugin.h"

using namespace std;
class PluginOffset
{
public:
    string OffsetName;
    float Start;
    float OffsetValue;
    float currentValue;
    
};

class PluginReplicator : public XMLPlugin
{
public:
	PluginReplicator()  
	{
		name="replicator";
		type=PLUGIN_TYPE_FUNCTION;

        non_linear_properties.push_back(NonLinearProperty(target_name,"target_name"));
        non_linear_properties.push_back(NonLinearProperty(ommit_indexes,"ommit_indexes"));
        non_linear_properties.push_back(NonLinearProperty(s_count,"count"));
	}

    vector<PluginOffset>    Offsets;
	string                  target_name;
    string                  ommit_indexes;
    string                  s_count;
    int                     count;
    int Iteration;
};