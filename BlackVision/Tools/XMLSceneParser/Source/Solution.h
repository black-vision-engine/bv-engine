#pragma once

#include <string>
#include <vector>
#include "rapidxml.hpp"
#include "helpers/auxiliary.hpp"
#include "helpers/KeyValue.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "TreeBuilder.h"

using namespace rapidxml;

namespace bv
{
class Solution
{
private:
	vector<string> ScenesToLoad;
    model::TimelineManager * timelineManager;
    BasicNodePtr root;
	
public:
  
    BasicNodePtr GetRoot(){return root;}
    bool LoadSolution(std::string name);
    void SetTimeline(model::TimelineManager * tm){timelineManager=tm;};
	Solution(model::TimelineManager * timelineManager);
	~Solution(void);
};

}
