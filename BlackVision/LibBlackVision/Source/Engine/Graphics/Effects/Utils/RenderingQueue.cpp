#include "stdafx.h"

#include "RenderingQueue.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"

#include "Memory/MemoryLeaks.h"


namespace bv {

namespace {

// ************************************
// FIXME: nrl - effect vs neffect
bool HasEffect( SceneNode * node )
{
    auto effect = node->GetNodeEffect();

    if( effect != nullptr && effect->GetType() != NodeEffectType::NET_DEFAULT )
    {
        return true;
    }

    auto neffect = node->GetNNodeEffect();

    if( neffect != nullptr && neffect->GetType() != nrl::NNodeEffectType::NNET_DEFAULT )
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
// FIXME: nrl - rendercontext added
float               RenderingQueue::ComputeNodeZ        ( SceneNode * node, nrl::NRenderContext * ctx )
{
    float z = 0.0f;

    if( HasEffect( node ) )
    {
        // Let effect compute z for itself instead of using bounding box.
        // @todo 
        //return z;
    }

    auto box = node->GetBoundingBox();
    if( box != nullptr )
    {
        auto camera = ctx->GetRenderer()->GetCamera();

        glm::vec3 boxCenter = glm::vec3( node->GetTransformable()->WorldTransform().Matrix() * glm::vec4( box->Center(), 1.0f ) );
        glm::vec3 cameraDir = camera->GetDirection();
        glm::vec3 cameraPos = camera->GetPosition();

        // Camera direction is normalized.
        z = glm::dot( ( boxCenter - cameraPos ), cameraDir );
    }

    return z;
}

// ***********************
//
bool                RenderingQueue::IsTransparent       ( SceneNode * node )
{
    auto renderableEntity = static_cast< bv::RenderableEntity * >( node->GetTransformable() );
    auto effect = renderableEntity->GetRenderableEffect();

    if( !effect )
    {
        return false;   // No effect. Return value is indifferent.
    }

    // FIXME: What if there're more passes then one.
    return effect->GetPass( 0 )->GetStateInstance()->GetAlphaState()->blendEnabled;
}


// ***********************
// FIXME: nrl - rendercontext added
void                RenderingQueue::QueueSingleNode     ( SceneNode * node, nrl::NRenderContext * ctx )
{
    if( !node->IsVisible() )
        return;

    BEGIN_CPU_QUEUEING_MESSURE( ctx->GetRenderer(), node );

    float z = ComputeNodeZ( node, ctx );
    
    if( IsTransparent( node ) || HasEffect( node ) )
    {
        // Farthest elements are at the beginning of vector.
        auto iterator = m_transparentNodes.begin();
        while( iterator != m_transparentNodes.end() )
        {
            if( z > iterator->second )
                break;
            iterator++;
        }

        m_transparentNodes.insert( iterator, std::make_pair( node, z ) );
    }
    else
    {
        // Nearest element are at the beginning of vector.
        auto iterator = m_opaqueNodes.begin();
        while( iterator != m_opaqueNodes.end() )
        {
            if( z < iterator->second )
                break;
            iterator++;
        }

        m_opaqueNodes.insert( iterator, std::make_pair( node, z ) );
    }

    END_CPU_QUEUEING_MESSURE( ctx->GetRenderer(), node );
}

// ***********************
// FIXME: nrl - rendercontext added
void                RenderingQueue::QueueNodeSubtree    ( SceneNode * node, nrl::NRenderContext * ctx )
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
// FIXME: nrl - rendercontext added
void                RenderingQueue::Render              ( nrl::NRenderContext * ctx )
{
    // Opaque objects from front to back.
    for( auto & renderItem : m_opaqueNodes )
    {
        RenderNode( renderItem.first, ctx );
    }

    // Transparent objects from back to front.
    for( auto & renderItem : m_transparentNodes )
    {
        RenderNode( renderItem.first, ctx );
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
// FIXME: nrl - rendercontext added
void                RenderingQueue::RenderNode          ( SceneNode * node, nrl::NRenderContext * ctx )
{
    // Function doesn't check if node is visible, beacause all nodes in m_transparentNodes
    // and m_opaqueNodes are visible. This have been checked in QueueNodeSubtree
    // and QueueSingleNode functions.

    BEGIN_MESSURE_GPU_PERFORMANCE( ctx->GetRenderer(), node );
    BEGIN_CPU_RENDER_MESSURE( ctx->GetRenderer(), node );

    // FIXME: nrl - implement more expressive api in NNodeRenderLogic so that bb and queue rendering is supported in a better way
    if( HasEffect( node ) )
    {
        nrl::NNodeRenderLogic::Render( node, ctx );
    }
    else
    {
        nrl::NNodeRenderLogic::RenderRoot( node->GetRepr(), ctx );
    }

    // FIXME: nrl - refactor bounding box rendering
    // FIXME: bb color should be configured externally to the node bot let it be that way for the time being
    if( node->IsSelected() && Cast< RenderableEntity* >( node->GetTransformable() )->GetRenderableEffect() != nullptr  )
    {
        nrl::NNodeRenderLogic::RenderBoundingBox( node, ctx );
    }

    END_CPU_RENDER_MESSURE( ctx->GetRenderer(), node );
    END_MESSURE_GPU_PERFORMANCE( ctx->GetRenderer(), node );
}

} // bv
