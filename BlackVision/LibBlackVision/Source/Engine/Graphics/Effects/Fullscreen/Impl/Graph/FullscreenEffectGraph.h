#pragma once

#include <set>
#include <vector>

#include "Engine/Graphics/Effects/FullScreen/Impl/Graph/FullscreenEffectGraphNode.h"


namespace bv {

class FullscreenEffectGraph
{
private:

    std::set< FullscreenEffectGraphNodePtr >    m_nodes;
   
    std::vector< FullscreenEffectGraphNodePtr > m_sourceNodes;
    FullscreenEffectGraphNodePtr                m_sinkNode;

public:

                                    FullscreenEffectGraph   ();
                                    ~FullscreenEffectGraph  ();

    FullscreenEffectGraphNodePtr    GetSinkNode             ();

    void                            InsertNode              ( FullscreenEffectGraphNodePtr node, const std::vector< FullscreenEffectGraphNodePtr > & predcessors );
    void                            SetSinkNode             ( FullscreenEffectGraphNodePtr node );

    void                            MarkSourceNode          ( FullscreenEffectGraphNodePtr node );

    const std::vector< FullscreenEffectGraphNodePtr > &     GetSourceNodes  () const;

	bool							IsSourceNode			( FullscreenEffectGraphNodePtr node ) const;

};

} //bv
