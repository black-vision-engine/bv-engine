#include "stdafx.h"
#include "RenderingQueue.h"


#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"


namespace bv
{


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
float               RenderingQueue::GetNodeZ            ( SceneNode * node, RenderLogicContext * ctx )
{
    float z = 0.0f;

    auto effect = node->GetNodeEffect();
    if( effect )
    {
        // Let effect compute z for itself instead of using bounding box.
        // @todo 
    }

    auto box = node->GetBoundingBox();
    if( box != nullptr )
    {
        auto camera = ctx->GetRenderer()->GetCamera();
        
        glm::vec3 boxCenter = box->Center();
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

    assert( effect );

    return effect->GetPass( 0 )->GetStateInstance()->GetAlphaState()->blendEnabled;
}


// ***********************
//
void                RenderingQueue::QueueSingleNode     ( SceneNode * node, RenderLogicContext * ctx )
{
    float z = GetNodeZ( node, ctx );
    
    if( IsTransparent( node ) )
    {
        auto iterator = m_transparentNodes.begin();

        while( iterator != m_transparentNodes.end() )
        {
            if( z > iterator->second )
            {
                iterator--;
                break;
            }
            iterator++;
        }

        m_transparentNodes.insert( iterator, std::make_pair( node, z ) );
    }
    else
    {
        auto iterator = m_opaqueNodes.begin();

        while( iterator != m_opaqueNodes.end() )
        {
            if( z < iterator->second )
            {
                iterator--;
                break;
            }
            iterator++;
        }

        m_opaqueNodes.insert( iterator, std::make_pair( node, z ) );
    }
}

//// ***********************
////
//void                RenderingQueue::QueueNodeSubtree    ( SceneNode * node, RenderLogicContext * ctx )
//{
//
//}
//
//// ***********************
////
//void                RenderingQueue::RenderQueue         ( RenderLogicContext * ctx )
//{
//
//}

// ***********************
//
void                RenderingQueue::ClearQueue          ()
{
    m_transparentNodes.clear();
    m_opaqueNodes.clear();
}

}	// bv