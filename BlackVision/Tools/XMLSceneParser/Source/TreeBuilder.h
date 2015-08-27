#pragma once

// todo:nie do koñca jestem pewien czy to dobrze
#pragma warning(disable:4996)

#include "BlackTree.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/BasicNode.h"
#include "Mathematics/Transform/MatTransform.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

namespace bv
{
	using namespace model;
	/*namespace model 
	{
		class BasicNode;
	}*/
	class TreeBuilder
	{
	private:
		static int TotalNodesCount;
		int NodesLoaded;
		BlackTree Tree;
        model::TimelineManager *        timelineManager;
        model::ITimeEvaluatorPtr        timeline_default;
        model::ITimeEvaluatorPtr        timeline_default_alpha;
        BlackNode*                      ProcessedNode;

		void PrepareNode();
	public:
		TreeBuilder(void);
		~TreeBuilder(void);
		model::BasicNodePtr             BuildTree                       (string path, model::TimelineManager * timelineManager);
		model::BasicNodePtr             SendTree                        (BlackNode &CurrentNode,int depth=0);
		bool                            AttachTransformPlugin            (model::BasicNodePtr node,XMLPlugin* plugin);
        bool                            AttachAlphaPlugin            (model::BasicNodePtr node,XMLPlugin* plugin);
		bool                            AttachRectanglePlugin           (model::BasicNodePtr node,XMLPlugin* plugin);
		bool                            AttachMaskPlugin           (model::BasicNodePtr node,XMLPlugin* plugin);
		bool                            AttachRingPlugin                (model::BasicNodePtr node,XMLPlugin* plugin);
		bool                            AttachPlugin                    (model::BasicNodePtr node, XMLPlugin* plugin);
		bool                            AttachSolidPlugin               (model::BasicNodePtr node,XMLPlugin* plugin);
		bool                            AttachLinearGradientPlugin      (model::BasicNodePtr node,XMLPlugin* plugin);
		bool                            AttachTexturePlugin             (model::BasicNodePtr node,XMLPlugin* plugin);
        bool                            AttachVideoInputPlugin            (model::BasicNodePtr node,XMLPlugin* plugin);
		bool                            AttachTexturePixelShader        (model::BasicNodePtr node);
		bool                            AttachTextPlugin                (model::BasicNodePtr node,XMLPlugin* plugin);
		bool                            AttachPrismPlugin                (model::BasicNodePtr node,XMLPlugin* plugin);
        bool                            AttachTimerPlugin               (model::BasicNodePtr node,XMLPlugin* plugin);
		bool                            AttachCrawlPlugin               (model::BasicNodePtr node,XMLPlugin* plugin);
		bool                            AttachPixelShader               (model::BasicNodePtr node, ParamVec4 color);
		bool                            AttachSequencePlugin            (model::BasicNodePtr node,XMLPlugin* plugin);
        bool                            AttachCubePlugin                (model::BasicNodePtr node,XMLPlugin* plugin);
        bool                            GenerateTimelines               ();
        bool                            AfterPlugins                    (model::BasicNodePtr node,BlackNode &CurrentNode);
        model::ITimeEvaluatorPtr        GetTimeline                   (string name); 
		
	};

}