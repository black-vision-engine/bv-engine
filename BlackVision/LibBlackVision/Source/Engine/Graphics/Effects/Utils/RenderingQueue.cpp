#include "stdafx.h"
#include "RenderingQueue.h"


#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"


namespace bv
{

namespace {

bool HasEffect( SceneNode * node )
{
    auto effect = node->GetNodeEffect();
    if( effect != nullptr && effect->GetType() != NodeEffectType::NET_DEFAULT )
        return true;
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
float               RenderingQueue::ComputeNodeZ        ( SceneNode * node, RenderLogicContext * ctx )
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
        return false;   // No effect. Return value is indifferent.

    // FIXME: What if there're more passes then one.
    return effect->GetPass( 0 )->GetStateInstance()->GetAlphaState()->blendEnabled;
}


// ***********************
//
void                RenderingQueue::QueueSingleNode     ( SceneNode * node, RenderLogicContext * ctx )
{
    if( !node->IsVisible() )
        return;

    float z = ComputeNodeZ( node, ctx );
    
    if( IsTransparent( node ) )
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
}

// ***********************
//
void                RenderingQueue::QueueNodeSubtree    ( SceneNode * node, RenderLogicContext * ctx )
{
    if( node->IsVisible() )
    {
        QueueSingleNode( node, ctx );

        // Don't render children if node has effect.
        if( !HasEffect( node ) )
        {
            for( int i = 0; i < node->NumChildNodes(); ++i )
            {
                QueueNodeSubtree( node->GetChild( i ), ctx );
            }
        }
    }
}

// ***********************
//
void                RenderingQueue::Render              ( RenderLogicContext * ctx )
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
//
void                RenderingQueue::RenderNode          ( SceneNode * node, RenderLogicContext * ctx )
{
    // Function doesn't check if node is visible, beacause all nodes in m_transparentNodes
    // and m_opaqueNodes are visible. This have been checked in QueueNodeSubtree
    // and QueueSingleNode functions.

    if( HasEffect( node ) )
    {
        auto effect = node->GetNodeEffect();
        effect->Render( node, ctx );
    }
    else
    {
        // Default render logic
        ctx->GetRenderLogic()->DrawNodeOnly( renderer( ctx ), node );
    }

    if( node->IsSelected() && Cast< RenderableEntity* >( node->GetTransformable() )->GetRenderableEffect() != nullptr  )
        ctx->GetRenderLogic()->RenderBoundingBox( node, ctx, node->GetBoundingBoxColor() );
}

}	// bv