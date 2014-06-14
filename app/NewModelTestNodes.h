#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
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

    static model::BasicNode *  CreateSolidRectNode      ( const std::string & name, float w, float h, const glm::vec3 & pos, const glm::vec4 col, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNode *  CreateTexturedRectNode   ( const std::string & name, float w, float h, const glm::vec3 & pos, const std::string & txFileName, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );

    static model::BasicNode *  CreateOverrideAlphaTest  ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );

    static model::BasicNode *  CreateGreenRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & nodeName );
    static model::BasicNode *  CreateGreenRectNodeNoAssert( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );

    static model::BasicNode *  CreateTexturedRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNode *  CreateTextureAnimationRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );

    static model::BasicNode *  CreateTextNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize = 0 );
    static model::BasicNode *  CreateTextWithShadowNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, const glm::vec3 shadowTranslation );
};

} //bv
