#pragma once

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

    static model::BasicNode *  CreateGreenRectNode( model::TimelineManager * timelineManager );
    static model::BasicNode *  CreateGreenRectNodeNoAssert( model::TimelineManager * timelineManager );

};

} //bv
