#pragma once

#include <utility>
#include <vector>


namespace bv
{

namespace nrl {

class nrl::RenderContext;

}

class SceneNode;

class RenderingQueue
{
    //typedef std::pair< SceneNode *, float > RenderItem;

    struct RenderItem
    {
        SceneNodeRepr *     Node;
        float               Depth;
        bool                UseEffect;

        RenderItem( SceneNodeRepr * node, float depth )
            : Node( node )
            , Depth( depth )
            , UseEffect( false )
        {}

        RenderItem( SceneNodeRepr * node, float depth, bool useEffect )
            : Node( node )
            , Depth( depth )
            , UseEffect( useEffect )
        {}
    };

private:

    std::vector< RenderItem >       m_transparentNodes;
    std::vector< RenderItem >       m_opaqueNodes;

public:

    RenderingQueue  ();
    ~RenderingQueue ();

    void                QueueSingleNode     ( SceneNode * node, nrl::RenderContext * ctx );
    void                QueueNodeSubtree    ( SceneNode * node, nrl::RenderContext * ctx );

    void                QueueNodeSubtree    ( SceneNodeRepr * nodeRepr, nrl::RenderContext * ctx );
    void                QueueSingleNode     ( SceneNodeRepr * nodeRepr, nrl::RenderContext * ctx );

    void                Render              ( nrl::RenderContext * ctx );

    void                ClearQueue          ();


    static float        ComputeNodeZ        ( SceneNode * node, nrl::RenderContext * ctx );
    static float        ComputeNodeZ        ( SceneNodeRepr * nodeRepr, nrl::RenderContext * ctx );
    static bool         IsTransparent       ( SceneNode * node );
    static bool         IsTransparent       ( SceneNodeRepr * nodeRepr );

private:
    void                RenderNode          ( RenderingQueue::RenderItem & renderItem, nrl::RenderContext * ctx );

    void                QueueTransparent    ( SceneNodeRepr * node, float z, bool useEffect = false );
    void                QueueOpaque         ( SceneNodeRepr * node, float z, bool useEffect = false );
};

DEFINE_UPTR_TYPE( RenderingQueue );

}	// bv
