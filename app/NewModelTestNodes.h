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

    model::BasicNode *  CreateGreenRectNode(  const model::PluginsManager * pluginsManager  );

};

} //bv
