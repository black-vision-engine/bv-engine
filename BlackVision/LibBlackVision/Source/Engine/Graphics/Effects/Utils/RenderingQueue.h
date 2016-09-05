#pragma once


namespace bv
{
class RenderLogicContext;
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

    void                QueueSingleNode     ( SceneNode * node, RenderLogicContext * ctx );
    void                QueueNodeSubtree    ( SceneNode * node, RenderLogicContext * ctx );

    void                Render              ( RenderLogicContext * ctx );

    void                ClearQueue          ();



    static float        ComputeNodeZ        ( SceneNode * node, RenderLogicContext * ctx );
    static bool         IsTransparent       ( SceneNode * node );

private:
    void                RenderNode          ( SceneNode * node, RenderLogicContext * ctx );
};


}	// bv
