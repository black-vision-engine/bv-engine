#include "stdafx.h"

#include "RenderingQueue.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"

#include "Memory/MemoryLeaks.h"


namespace bv { namespace {

// ************************************
//
bool HasEffect( SceneNode * node )
{
    auto neffect = node->GetNodeEffect();

    if( node->IsNodeEffectEnabled() && neffect != nullptr && neffect->GetType() != NodeEffectType::NET_DEFAULT )
    {
        return true;
    }

    return false;
}


}   // anounymous

// ***********************
//
RenderingQueue::RenderingQueue()
{}

// ***********************
//
RenderingQueue::~RenderingQueue()
{}


// ***********************
//
float               RenderingQueue::ComputeNodeZ        ( SceneNode * node, RenderContext * ctx )
{
    if( HasEffect( node ) )
    {
        // Let effect compute z for itself instead of using bounding box.
        auto effect = node->GetNodeEffect();
        if( effect )
        {
            if( effect->IsDepthOverriden_DIRTY_DESIGN_HACK() )
            {
                return effect->GetDepth_DIRTY_DESIGN_HACK();
            }
            // In other cases depth is computet from node bounding box.
        }
    }

    return ComputeNodeZ( node->GetRepr(), ctx );
}

// ***********************
//
float               RenderingQueue::ComputeNodeZ        ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    float z = 0.0f;

    auto box = nodeRepr->GetBoundingBox();
    if( box != nullptr )
    {
        auto camera = ctx->GetRenderer()->GetCamera();

        glm::vec3 boxCenter = glm::vec3( nodeRepr->GetTransformable()->WorldTransform().Matrix() * glm::vec4( box->Center(), 1.0f ) );
        const glm::vec3 & cameraDir = camera->GetDirection();
        const glm::vec3 & cameraPos = camera->GetPosition();

        // Camera direction is normalized. (It should be)
        z = glm::dot( ( boxCenter - cameraPos ), cameraDir );
    }

    return z;
}

// ***********************
//
bool                RenderingQueue::IsTransparent       ( SceneNode * node )
{
    if( HasEffect( node ) )
    {
        auto effect = node->GetNodeEffect();

        if( effect )
        {
            return effect->IsBlendable_DIRTY_DESIGN_HACK();
        }
        assert( !"Shouldn't be here" );
        return true;
    }
    else
    {
        return IsTransparent( node->GetRepr() );
    }
}

// ***********************
//
bool                RenderingQueue::IsTransparent       ( SceneNodeRepr * nodeRepr )
{
    auto renderableEntity = static_cast<bv::RenderableEntity *>( nodeRepr->GetTransformable() );
    auto effect = renderableEntity->GetRenderableEffect();

    if( !effect )
    {
        return false;   // No effect. Return value is indifferent.
    }

    // FIXME: What if there're more passes then one.
    return effect->GetPass( 0 )->GetStateInstance()->GetAlphaState()->blendEnabled;
}


// ***********************
// 
void                RenderingQueue::QueueSingleNode     ( SceneNode * node, RenderContext * ctx )
{
    BEGIN_CPU_QUEUEING_MESSURE( ctx->GetRenderer(), node->GetRepr() );

    float z = ComputeNodeZ( node, ctx );
    bool hasEffect = HasEffect( node );
    
    if( IsTransparent( node ) )
    {
        QueueTransparent( node->GetRepr(), z, hasEffect );
    }
    else
    {
        QueueOpaque( node->GetRepr(), z, hasEffect );
    }

    // FIXME: nrl - refactor bounding box rendering
    // FIXME: bb color should be configured externally to the node bot let it be that way for the time being
    if( node->IsSelected() && Cast< RenderableEntity* >( node->GetTransformable() )->GetRenderableEffect() != nullptr  )
    {
        NodeRenderLogic::RenderBoundingBox( node, ctx );
    }

    END_CPU_QUEUEING_MESSURE( ctx->GetRenderer(), node->GetRepr() );
}

// ***********************
//
void                RenderingQueue::QueueSingleNode     ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    BEGIN_CPU_QUEUEING_MESSURE( ctx->GetRenderer(), nodeRepr );

    float z = ComputeNodeZ( nodeRepr, ctx );

    if( IsTransparent( nodeRepr ) )
    {
        QueueTransparent( nodeRepr, z );
    }
    else
    {
        QueueOpaque( nodeRepr, z );
    }

    END_CPU_QUEUEING_MESSURE( ctx->GetRenderer(), nodeRepr );
}

// ***********************
//
void                RenderingQueue::QueueNodeSubtree    ( SceneNode * node, RenderContext * ctx )
{
    if( node->IsVisible() )
    {
        QueueSingleNode( node, ctx );

        // Don't render children if node has effect.
        if( !HasEffect( node ) )
        {
            for( Int32 i = 0; i < ( Int32 )node->NumChildNodes(); ++i )
            {
                QueueNodeSubtree( node->GetChild( i ), ctx );
            }
        }
    }
}

// ***********************
//
void                RenderingQueue::QueueNodeSubtree    ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    QueueSingleNode( nodeRepr, ctx );

    for( Int32 i = 0; i < (Int32)nodeRepr->NumChildNodes(); ++i )
    {
        QueueNodeSubtree( nodeRepr->GetChild( i ), ctx );
    }
}

// ***********************
//
void                RenderingQueue::QueueTransparent    ( SceneNodeRepr * node, float z, bool useEffect )
{
    // Farthest elements are at the beginning of vector.
    auto iterator = m_transparentNodes.begin();
    while( iterator != m_transparentNodes.end() )
    {
        if( z > iterator->Depth )
            break;
        iterator++;
    }

    m_transparentNodes.insert( iterator, RenderItem( node, z, useEffect ) );
}

// ***********************
//
void                RenderingQueue::QueueOpaque         ( SceneNodeRepr * node, float z, bool useEffect )
{
    // Nearest element are at the beginning of vector.
    auto iterator = m_opaqueNodes.begin();
    while( iterator != m_opaqueNodes.end() )
    {
        if( z < iterator->Depth )
            break;
        iterator++;
    }

    m_opaqueNodes.insert( iterator, RenderItem( node, z, useEffect ) );
}

// ***********************
//
void                RenderingQueue::Render              ( RenderContext * ctx )
{
    // Opaque objects from front to back.
    for( auto & renderItem : m_opaqueNodes )
    {
        RenderNode( renderItem, ctx );
    }

    // Transparent objects from back to front.
    for( auto & renderItem : m_transparentNodes )
    {
        RenderNode( renderItem, ctx );
    }
}

// ***********************
//
void                RenderingQueue::ClearQueue          ()
{
    m_transparentNodes.clear();
    m_opaqueNodes.clear();
}

// ========================================================================= //
// Private rendering helpers
// ========================================================================= //

// ***********************
//
void                RenderingQueue::RenderNode          ( RenderingQueue::RenderItem & renderItem, RenderContext * ctx )
{
    // Function doesn't check if node is visible, beacause all nodes in m_transparentNodes
    // and m_opaqueNodes are visible. This have been checked in QueueNodeSubtree
    // and QueueSingleNode functions.

    //BEGIN_MESSURE_GPU_PERFORMANCE( ctx->GetRenderer(), renderItem.Node );
    BEGIN_CPU_RENDER_MESSURE( ctx->GetRenderer(), renderItem.Node );

    // FIXME: nrl - implement more expressive api in NodeRenderLogic so that bb and queue rendering is supported in a better way
    if( renderItem.UseEffect )
    {
        auto ownerNode = renderItem.Node->GetOwnerNode();
        NodeRenderLogic::Render( ownerNode, ctx );
    }
    else
    {
        NodeRenderLogic::RenderRoot( renderItem.Node, ctx );
    }

    END_CPU_RENDER_MESSURE( ctx->GetRenderer(), renderItem.Node );
    //END_MESSURE_GPU_PERFORMANCE( ctx->GetRenderer(), renderItem.Node );
}

} // bv
