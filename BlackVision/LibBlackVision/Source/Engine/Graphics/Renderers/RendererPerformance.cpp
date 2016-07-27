#include "stdafx.h"
#include "RendererPerformance.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTimeQuery.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"


namespace bv
{



// ***********************
//
void                RendererPerformance::FreeAllPDResources  ( RenderableEntity * renderable )
{
    delete m_timeQueryMap[ renderable ];
    m_timeQueryMap.erase( renderable );
}

// ***********************
//
void        RendererPerformance::BeginGPUQuery               ( SceneNode * sceneNode )
{
    auto timer = GetTimeQuery( static_cast< RenderableEntity * >( sceneNode->GetTransformable() ) );
    timer->BeginQuery();
}

// ***********************
//
void        RendererPerformance::EndGPUQuery                 ( SceneNode * sceneNode )
{
    auto timer = GetTimeQuery( static_cast< RenderableEntity * >( sceneNode->GetTransformable() ) );
    timer->EndQuery();
}

// ***********************
//
void        RendererPerformance::QueryPreviousGPUResult      ( SceneNode * sceneNode )
{
    auto timer = GetTimeQuery( static_cast< RenderableEntity * >( sceneNode->GetTransformable() ) );
    auto result = timer->QueryResult();

    auto perfData = sceneNode->GetPerformanceData();
    perfData->GPURenderDuration = (UInt32)result;
}


// ***********************
//
PdrTimeQuery *      RendererPerformance::GetTimeQuery        ( RenderableEntity * renderable )
{
    PdrTimeQuery * timer = m_timeQueryMap[ renderable ];
    if( timer == nullptr )
    {
        timer = m_timeQueryMap[ renderable ] = new PdrTimeQuery();
    }

    return timer;
}

}	// bv

