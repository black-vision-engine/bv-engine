#include "stdafx.h"
#include "AssetsThread.h"

#include "Assets/AssetManager.h"
#include "Engine/Events/EventManager.h"
#include "AssetAsyncLoadFinishedEvent.h"


namespace bv
{


// ***********************
//
AssetsThread::AssetsThread()
{
    m_thread = std::thread( &AssetsThread::LoadingThread, this );
}

// ***********************
//
AssetsThread::~AssetsThread()
{
    EndThread();
    m_thread.join();
}

// ***********************
//
void            AssetsThread::QueueRequest      ( LoadAssetRequest && request )
{
    assert( request.Descriptor );       // Note nullptr here will end this thread.

    m_assetRequests.Push( std::move( request ) );
}

// ***********************
//
void            AssetsThread::EndThread         ()
{
    LoadAssetRequest emptyRequest;
    emptyRequest.Descriptor = nullptr;

    m_assetRequests.Push( std::move( emptyRequest ) );
}

// ***********************
//
void            AssetsThread::LoadingThread     ()
{
    LoadAssetRequest request;

    while( true )
    {
        m_assetRequests.WaitAndPop( request );

        if( request.Descriptor == nullptr )
            break;

        // Load asset here
        AssetConstPtr asset = AssetManager::GetInstance().LoadAsset( request.Descriptor );

        // Send event
        AssetAsyncLoadFinishedEventPtr loadedEvt = std::make_shared< AssetAsyncLoadFinishedEvent >( asset, std::move( request ) );
        GetDefaultEventManager().ConcurrentQueueEvent( loadedEvt );
    }
}


}	// bv