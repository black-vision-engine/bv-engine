#pragma once

#include <list>
#include <vector>
#include<string>
#include "plugins/BlackNode.h"
#include "plugins/BlackTimeline.h"
#include "helpers/KeyValue.h"
#include "plugins/SceneMetaTags.h"

#include "plugins\PluginExtrude.h"
#include "plugins\PluginAlpha.h"
#include "plugins\PluginGeometry.h"
#include "plugins\PluginLinearGradient.h"
#include "plugins\PluginRadialGradient.h"
#include "plugins\PluginRectangle.h"
#include "plugins\PluginRing.h"
#include "plugins\PluginSequence.h"
#include "plugins\PluginSolid.h"
#include "plugins\PluginText.h"
#include "plugins\PluginTimer.h"
#include "plugins\PluginTexture.h"
#include "plugins\PluginInstance.h"
#include "plugins\BlackClass.h"
#include "plugins\PluginReplicator.h"
#include "plugins\PluginCrawl.h"
#include "plugins\PluginMask.h"
#include "plugins\PluginPrism.h"


using namespace std;


class Scena{

public:


	Scena(){NodeCount=0;MainNode.name = "Root";};
	int NodeCount;
	BlackNode MainNode;
    vector<BlackTimeline*> Timelines;
    vector<BlackClass*> Classes;
    vector<KeyValue*> MetaTags;
    SceneMetaTags Meta;
    void ParseMetaTags();

	BlackNode* FindNode(BlackNode *LookFor,string name);
	BlackNode* FindNode(string name){
		return FindNode(&MainNode,name);
	}
};

