#pragma once

#include <utility>
#include <vector>


namespace bv
{

class RenderContext;
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

    void                QueueSingleNode     ( SceneNode * node, RenderContext * ctx );
    void                QueueNodeSubtree    ( SceneNode * node, RenderContext * ctx );

    void                QueueNodeSubtree    ( SceneNodeRepr * nodeRepr, RenderContext * ctx );
    void                QueueSingleNode     ( SceneNodeRepr * nodeRepr, RenderContext * ctx );

    void                Render              ( RenderContext * ctx );

    void                ClearQueue          ();


    static float        ComputeNodeZ        ( SceneNode * node, RenderContext * ctx );
    static float        ComputeNodeZ        ( SceneNodeRepr * nodeRepr, RenderContext * ctx );
    static bool         IsTransparent       ( SceneNode * node );
    static bool         IsTransparent       ( SceneNodeRepr * nodeRepr );

private:
    void                RenderNode          ( RenderingQueue::RenderItem & renderItem, RenderContext * ctx );

    void                QueueTransparent    ( SceneNodeRepr * node, float z, bool useEffect = false );
    void                QueueOpaque         ( SceneNodeRepr * node, float z, bool useEffect = false );
};

DEFINE_UPTR_TYPE( RenderingQueue );

} // bv
