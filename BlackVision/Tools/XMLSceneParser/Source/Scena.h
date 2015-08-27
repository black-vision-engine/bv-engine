#pragma once

#include <list>
#include <vector>
#include<string>

//basic classes
#include "plugins/base/BlackNode.h"
#include "plugins/base/BlackTimeline.h"
#include "helpers/KeyValue.h"
#include "plugins/base/SceneMetaTags.h"



#include "plugins/base/BlackClass.h"

//geometry
#include "plugins/geometry/PluginRectangle.h"
#include "plugins/geometry/PluginPrism.h"
#include "plugins/geometry/PluginRing.h"
#include "plugins/geometry/PluginText.h"
#include "plugins/geometry/PluginTimer.h"
#include "plugins/geometry/PluginExtrude.h"
#include "plugins/geometry/PluginCircle.h"
#include "plugins/geometry/PluginCone.h"
#include "plugins/geometry/PluginCube.h"
#include "plugins/geometry/PluginEllipse.h"
#include "plugins/geometry/PluginRoundedRectangle.h"
#include "plugins/geometry/PluginSphere.h"
#include "plugins/geometry/PluginTriangle.h"




//logic
#include "plugins/logic/PluginAlpha.h"
#include "plugins/logic/PluginReplicator.h"
#include "plugins/logic/PluginCrawl.h"
#include "plugins/logic/PluginMask.h"
#include "plugins/logic/PluginInstance.h"
//texture
#include "plugins/texture/PluginLinearGradient.h"
#include "plugins/texture/PluginRadialGradient.h"
#include "plugins/texture/PluginSequence.h"
#include "plugins/texture/PluginSolid.h"
#include "plugins/texture/PluginTexture.h"
#include "plugins/texture/PluginVideoInput.h"

//transform

#include "plugins/transform/PluginTransform.h"










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

