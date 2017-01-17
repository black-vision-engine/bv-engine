#include "stdafx.h"
#include "RendererPerformance.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTimeQuery.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/SceneGraph/Scene.h"

#include "tools/HRTimer.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

extern HighResolutionTimer GTimer;

// ***********************
//
RendererPerformance::RendererPerformance()
{
    m_windowSize = 100;
    m_samplesAccumulated = 0;
}


// ***********************
//
void                RendererPerformance::FreeAllPDResources  ( RenderableEntity * renderable )
{
    delete m_timeQueryMap[ renderable ];
    m_timeQueryMap.erase( renderable );
}

// ***********************
//
void        RendererPerformance::BeginGPUQuery               ( SceneNodeRepr * sceneNode )
{
    auto timer = GetTimeQuery( static_cast< RenderableEntity * >( sceneNode->GetTransformable() ) );
    timer->BeginQuery();
}

// ***********************
//
void        RendererPerformance::EndGPUQuery                 ( SceneNodeRepr * sceneNode )
{
    auto timer = GetTimeQuery( static_cast< RenderableEntity * >( sceneNode->GetTransformable() ) );
    timer->EndQuery();
}

// ***********************
//
void        RendererPerformance::QueryPreviousGPUResult      ( SceneNodeRepr * sceneNode )
{
    auto timer = GetTimeQuery( static_cast< RenderableEntity * >( sceneNode->GetTransformable() ) );
    auto result = timer->QueryResult();

    auto perfData = sceneNode->GetPerformanceData();
    perfData->GPURenderAccum += (UInt32)result;
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

// ***********************
//
void                RendererPerformance::AverageScenePerformanceData  ( Scene * scene )
{
    if( m_samplesAccumulated >= m_windowSize )
    {
        AverageTreePerformanceData( scene->GetRoot()->GetRepr() );
        m_samplesAccumulated = 0;
    }
    else
    {
        m_samplesAccumulated++;
    }
}

// ***********************
//
double      RendererPerformance::BeginCPURenderMessure       ()
{
    return GTimer.CurElapsed();
}

// ***********************
//
void        RendererPerformance::EndCPURenderMessure         ( SceneNodeRepr * sceneNode, double timeStart )
{
    sceneNode->GetPerformanceData()->CPURenderAccum += GTimer.CurElapsed() - timeStart;
}

// ***********************
//
double      RendererPerformance::BeginCPUQueueingMessure     ()
{
    return GTimer.CurElapsed();
}

// ***********************
//
void        RendererPerformance::EndCPUQueueingMessure       ( SceneNodeRepr * sceneNode, double timeStart )
{
    sceneNode->GetPerformanceData()->SortNodeAccum += GTimer.CurElapsed() - timeStart;
}

// ***********************
//
void                RendererPerformance::AverageTreePerformanceData  ( SceneNodeRepr * node )
{
    auto data = node->GetPerformanceData();
    data->CPURenderDuration = (double)data->CPURenderAccum / m_windowSize;
    data->GPURenderDuration = ( (double)data->GPURenderAccum / m_windowSize ) / 1000000000.0;   // Convert to seconds.
    data->SortNodeDuration = (double)data->SortNodeAccum / m_windowSize;

    data->CPURenderAccum = 0;
    data->GPURenderAccum = 0;
    data->SortNodeAccum = 0;

    for( Int32 i = 0; i < ( Int32 )node->NumChildNodes(); ++i )
    {
        AverageTreePerformanceData( node->GetChild( i )->GetRepr() );
    }
}

}	// bv

