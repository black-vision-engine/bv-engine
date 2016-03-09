#pragma once

#include <set>
#include <vector>

#include "Engine/Graphics/Effects/FullScreen/Impl/Graph/FullscreenEffectGraphNode.h"
#include "Engine/Graphics/Effects/FullScreen/Impl/Graph/InputFullscreenEffectGraphNode.h"

namespace bv {

class FullscreenEffectGraph
{
private:

    std::set< FullscreenEffectGraphNodePtr >    m_nodes; // Only for checking proper graph structure.
   
    std::vector< FullscreenEffectGraphNodePtr > m_sourceNodes;
    FullscreenEffectGraphNodePtr                m_sinkNode;

public:

                                    FullscreenEffectGraph   ();
                                    ~FullscreenEffectGraph  ();

    FullscreenEffectGraphNodePtr    GetSinkNode             ();

    void                            AddNode                 ( FullscreenEffectGraphNodePtr node );
    void                            InsertNode              ( FullscreenEffectGraphNodePtr node, const std::vector< FullscreenEffectGraphNodePtr > & predcessors );
    void                            SetSinkNode             ( FullscreenEffectGraphNodePtr node );

    void                            MarkSourceNode          ( FullscreenEffectGraphNodePtr node );

    const std::vector< FullscreenEffectGraphNodePtr > &     GetSourceNodes  () const;
    std::vector< FullscreenEffectGraphNodePtr > &           GetSourceNodes  ();

	bool							IsSourceNode			( FullscreenEffectGraphNodePtr node ) const;
    SizeType                        SourceNodeIndex         ( FullscreenEffectGraphNodePtr node ) const;

    void                            FlattenGraph            ();

private:

    FullscreenEffectGraphNodePtr   FlattenNode             ( FullscreenEffectGraphNodePtr node );

};

} //bv
