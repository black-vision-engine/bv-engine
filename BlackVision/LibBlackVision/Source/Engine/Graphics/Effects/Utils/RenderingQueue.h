#pragma once

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"


namespace bv
{


class RenderingQueue
{
    typedef std::pair< SceneNode *, float > RenderItem;

private:

    std::vector< RenderItem >       m_transparentNodes;
    std::vector< RenderItem >       m_opaqueNodes;

public:
    RenderingQueue();
    ~RenderingQueue();

    void                QueueSingleNode     ( SceneNode * node, RenderLogicContext * ctx );
    void                QueueNodeSubtree    ( SceneNode * node, RenderLogicContext * ctx );

    void                RenderQueue         ( RenderLogicContext * ctx );

    void                ClearQueue          ();



    static float        GetNodeZ            ( SceneNode * node, RenderLogicContext * ctx );
    static bool         IsTransparent       ( SceneNode * node );
};


}	// bv
