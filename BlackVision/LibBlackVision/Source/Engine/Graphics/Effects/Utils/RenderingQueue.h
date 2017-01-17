#pragma once

#include <utility>
#include <vector>


namespace bv
{

namespace nrl {

class nrl::NRenderContext;

}

class SceneNode;

class RenderingQueue
{
    typedef std::pair< SceneNode *, float > RenderItem;

private:

    std::vector< RenderItem >       m_transparentNodes;
    std::vector< RenderItem >       m_opaqueNodes;

public:
    RenderingQueue();
    ~RenderingQueue();

    void                QueueSingleNode     ( SceneNode * node, nrl::NRenderContext * ctx );
    void                QueueNodeSubtree    ( SceneNode * node, nrl::NRenderContext * ctx );

    void                QueueNodeSubtree    ( SceneNodeRepr * nodeRepr, nrl::NRenderContext * ctx );
    void                QueueSingleNode     ( SceneNodeRepr * nodeRepr, nrl::NRenderContext * ctx );

    void                Render              ( nrl::NRenderContext * ctx );

    void                ClearQueue          ();



    static float        ComputeNodeZ        ( SceneNode * node, nrl::NRenderContext * ctx );
    static float        ComputeNodeZ        ( SceneNodeRepr * nodeRepr, nrl::NRenderContext * ctx );
    static bool         IsTransparent       ( SceneNode * node );
    static bool         IsTransparent       ( SceneNodeRepr * nodeRepr );

private:
    void                RenderNode          ( SceneNode * node, nrl::NRenderContext * ctx );

    void                QueueTransparent    ( SceneNode * node, float z );
    void                QueueOpaque         ( SceneNode * node, float z );
};


}	// bv
