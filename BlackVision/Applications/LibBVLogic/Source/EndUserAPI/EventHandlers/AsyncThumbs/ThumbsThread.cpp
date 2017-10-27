#include "stdafxBVApp.h"
#include "ThumbsThread.h"

#include "Assets/AssetManager.h"
#include "Engine/Events/EventManager.h"

#include "ProjectManager.h"




namespace bv
{


// ***********************
//
ThumbsThread::ThumbsThread()
    : m_assetRequests( 15000 )
{
    m_thread = std::thread( &ThumbsThread::LoadingThread, this );
}

// ***********************
//
ThumbsThread::~ThumbsThread()
{
    EndThread();
    m_thread.join();
}

// ***********************
//
void            ThumbsThread::QueueRequest      ( LoadThumbRequest && request )
{
    assert( !request.EndThread );       // Note nullptr here will end this thread.

    m_assetRequests.WaitAndPush( std::move( request ) );
}

// ***********************
//
void            ThumbsThread::EndThread         ()
{
    LoadThumbRequest emptyRequest;
    emptyRequest.EndThread = true;

    m_assetRequests.Clear();
    m_assetRequests.WaitAndPush( std::move( emptyRequest ) );
}

// ***********************
//
void            ThumbsThread::LoadingThread     ()
{
    LoadThumbRequest request;

    while( true )
    {
        m_assetRequests.WaitAndPop( request );

        if( request.EndThread )
            break;

        GenerateThumbnails( request );
    }
}

// ***********************
//
void            ThumbsThread::GenerateThumbnails    ( const LoadThumbRequest & request )
{
    auto & projectName = request.ProjectName;
    auto & categoryName = request.Category;
    auto & path = request.Path;

    auto pm = ProjectManager::GetInstance();
    auto aps = pm->ListAssetsPaths( projectName, categoryName, path, false );

    std::vector< std::pair< ThumbnailConstPtr, AssetDescConstPtr > > thumbs;

    for( auto & ap : aps )
    {
        ap = Path( ap.Str().substr( ap.Str().find_first_not_of( categoryName ) ) ); // FIXME: Ugly part of code. Add some function parsing path to category name and path.
        auto desc = pm->GetAssetDesc( projectName, categoryName, ap );

        if( desc )
        {
            auto thumb = AssetManager::GetInstance().LoadThumbnail( desc );

            if( thumb != nullptr )  // Fixme Inform editor that some thumbs can't be generated.
                thumbs.push_back( std::make_pair( thumb, desc ) );
        }
    }

    SimpleJsonSerializeObject ser;


    if( !thumbs.empty() )
    {
        PrepareResponseTemplate( ser, InfoEvent::Command::GetAssetThumbnail, request.EventID, true );

        ser.EnterArray( "thumbnails" );

        for( auto t : thumbs )
        {
            t.first->Serialize( ser );
            t.second->Serialize( ser );
        }

        ser.ExitChild(); // thumbnails
    }
    else
    {
        ErrorResponseTemplate( ser, InfoEvent::Command::GetAssetThumbnail, request.EventID, "Cannot find asset." );
    }

    SendResponse( ser, request.SocketID, request.EventID );
}


}	// bv