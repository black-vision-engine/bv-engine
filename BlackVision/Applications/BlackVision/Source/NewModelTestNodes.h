#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/BasicNode.h"
#include "Mathematics/glm_inc.h"

namespace bv {

namespace model 
{
    class BasicNode;
    class PluginsManager;
    class TimelineManager;
}

// ********************************
class SimpleNodesFactory
{
public:

    static model::BasicNodePtr  CreateSolidRectNode         ( const std::string & name, float w, float h, const glm::vec3 & pos, const glm::vec4 col, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateTexturedRectNode      ( const std::string & name, float w, float h, const glm::vec3 & pos, const std::string & txFileName, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
                           
    static model::BasicNodePtr  CreateOverrideAlphaTest     ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateOverrideNodeMaskTest  ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateOverrideNodeMaskTest1 ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
                           
    static model::BasicNodePtr  CreateGreenRectNode         ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & nodeName );
    static model::BasicNodePtr  CreateGreenRectNodeNoAssert ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );

    static model::BasicNodePtr  CreateOlafRectNode          ( model::TimelineManager *timelineManager, model::ITimeEvaluatorPtr timeEvaulator);
    static model::BasicNodePtr  CreateCreedRectNode(model::TimelineManager *timelineManager, model::ITimeEvaluatorPtr timeEvaulator);
    static model::BasicNodePtr  CreateCreedTextNode(model::TimelineManager *timelineManager, model::ITimeEvaluatorPtr timeEvaulator);
    static model::BasicNodePtr  CreateCreedTimerNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, bool useAlphaMask  );
	static model::BasicNodePtr  CreateCreedPrismNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, float offset);
	static model::BasicNodePtr  CreateCreedColoredPrismNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, float offset);
	static model::BasicNodePtr  CreateCreedTexturedPrismNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, float offset);
	static model::BasicNodePtr  CreateCreedGradedPrismNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, float offset);
	
	static model::BasicNodePtr	CreateCreedColoredPieChartNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, float offset );
	static model::BasicNodePtr	CreateCreedGradedPieChartNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, float offset );

    static model::BasicNodePtr  CreateTexturedRectNode      ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );
    static model::BasicNodePtr  CreateTexturedTextNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );
    static model::BasicNodePtr  CreateTextureAnimationRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );
                           
    static model::BasicNodePtr  CreateTextNode              ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, bool useAlphaMask );
    static model::BasicNodePtr  CreateTextWithShadowNode    ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, const glm::vec3 shadowTranslation );

    static model::BasicNodePtr  CreateTimerNode             ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, bool useAlphaMask  );

    static model::BasicNodePtr  CreateHeightMapNode         ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );

	static model::BasicNodePtr	CreateCrawlerNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );

	static model::BasicNodePtr	CreateBasicShapesTestNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
};

} //bv
