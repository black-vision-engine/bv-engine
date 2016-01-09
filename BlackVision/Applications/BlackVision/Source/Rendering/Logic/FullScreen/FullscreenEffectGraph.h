#pragma once

#include <set>
#include <vector>

#include "Rendering/Logic/FullScreen/FullscreenEffectGraphNode.h"


namespace bv {

class FullscreenEffectGraph
{
private:

    std::set< FullscreenEffectGraphNodePtr >    m_nodes;

    FullscreenEffectGraphNodePtr                m_sinkNode;

public:

                                    FullscreenEffectGraph   ();
                                    ~FullscreenEffectGraph  ();

    FullscreenEffectGraphNodePtr    GetSinkNode             ();

    void                            InsertNode              ( FullscreenEffectGraphNodePtr node, const std::vector< FullscreenEffectGraphNodePtr > & predcessors );
    void                            SetSinkNode             ( FullscreenEffectGraphNodePtr node );

};

} //bv
