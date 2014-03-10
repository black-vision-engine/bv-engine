#pragma once

namespace bv {

namespace model 
{
    class BasicNode;
    class PluginsManager;
}

// ********************************
class SimpleNodesFactory
{
public:

    static model::BasicNode *  CreateGreenRectNode();

};

} //bv
