#pragma once

#include <vector>


namespace bv {

class NodeEffectRenderLogic;
class NodeEffectRenderLogicTr;
class SceneNode;


class NodeEffectRenderLogicSelector
{
private:

    std::vector< NodeEffectRenderLogic * >      m_nodeRenderLogicVec;
    std::vector< NodeEffectRenderLogicTr * >    m_nodeRenderLogicVecTr;

public:

                            NodeEffectRenderLogicSelector   ();
                            ~NodeEffectRenderLogicSelector  ();

    NodeEffectRenderLogic * GetNodeEffectRenderLogic        ( SceneNode * node );
    NodeEffectRenderLogicTr * GetNodeEffectRenderLogicTr    ( SceneNode * node );

};

} //bv
