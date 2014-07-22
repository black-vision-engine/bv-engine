#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/BasicNode.h"
#include <glm/glm.hpp>

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

    static model::BasicNodePtr  CreateSolidRectNode      ( const std::string & name, float w, float h, const glm::vec3 & pos, const glm::vec4 col, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateTexturedRectNode   ( const std::string & name, float w, float h, const glm::vec3 & pos, const std::string & txFileName, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
                           
    static model::BasicNodePtr  CreateOverrideAlphaTest  ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
                           
    static model::BasicNodePtr  CreateGreenRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & nodeName );
    static model::BasicNodePtr  CreateGreenRectNodeNoAssert( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );
                           
    static model::BasicNodePtr  CreateTexturedRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );
    static model::BasicNodePtr  CreateTextureAnimationRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );
                           
    static model::BasicNodePtr  CreateTextNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, bool useAlphaMask );
    static model::BasicNodePtr  CreateTextWithShadowNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, const glm::vec3 shadowTranslation );

    static model::BasicNodePtr  CreateTimerNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, bool useAlphaMask  );
};

} //bv
