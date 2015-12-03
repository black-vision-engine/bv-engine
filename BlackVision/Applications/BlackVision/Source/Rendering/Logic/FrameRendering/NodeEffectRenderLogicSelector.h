#pragma once

#include <vector>


namespace bv {

class NodeEffectRenderLogic;
class SceneNode;


class NodeEffectRenderLogicSelector
{
private:

    std::vector< NodeEffectRenderLogic * >  m_nodeRenderLogicVec;

public:

                            NodeEffectRenderLogicSelector   ();
                            ~NodeEffectRenderLogicSelector  ();

    NodeEffectRenderLogic * GetNodeEffectRenderLogic        ( SceneNode * node );

};

} //bv
