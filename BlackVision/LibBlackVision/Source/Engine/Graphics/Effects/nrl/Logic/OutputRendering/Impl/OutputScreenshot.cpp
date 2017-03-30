#include "stdafx.h"

#include "OutputScreenshot.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"

#include "LibImage.h"

#include "System/Path.h"
#include "IO/DirIO.h"
#include "Tools/Profiler/HerarchicalProfiler.h"

#include "Engine/Events/EventManager.h"
#include "Engine/Events/Events.h"

#include <thread>


namespace bv {
namespace nrl {


// ***********************
//
void SaveImageToFileFunction    ( const std::string & filePath, MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp, bool sendEvent, bool sendAsync )
{
    bool result = image::SaveBMPImage( filePath, data, width, height, bpp );

    if( sendEvent )
    {
        ScreenShotRenderedEventPtr renderedEvent = std::make_shared< ScreenShotRenderedEvent >();
        renderedEvent->FilePath = filePath;
        renderedEvent->Result = result;

        if( sendAsync )
            GetDefaultEventManager().ConcurrentQueueEvent( renderedEvent );
        else
            GetDefaultEventManager().TriggerEvent( renderedEvent );
    }
}


// *********************************
//
OutputScreenshot::OutputScreenshot()
{}

// *********************************
//
void    OutputScreenshot::ProcessFrameData  ( ScreenshotInfo & screenshotInfo, NRenderContext * ctx, const RenderChannel * channel )
{
    if( ReadbackNeeded( screenshotInfo ) )
    {
        std::string newFilePath = screenshotInfo.FilePath + std::to_string( screenshotInfo.AllFrames - screenshotInfo.RemainingFrames ) + ".bmp";

        assert( channel->IsActive() );

        if( channel->IsActive() )
        {
            Texture2DPtr frame;
            renderer( ctx )->ReadColorTexturSync( 0, channel->GetActiveRenderTarget(), frame );

            auto chunk = frame->GetData();

            if( screenshotInfo.SaveToFileAsync )
            {
                std::thread asyncWrite( SaveImageToFileFunction, newFilePath, chunk, frame->GetWidth(), frame->GetHeight(), 32, screenshotInfo.OnRenderedEvent, false );
                asyncWrite.detach();
            }
            else
            {
                SaveImageToFileFunction( newFilePath, chunk, frame->GetWidth(), frame->GetHeight(), 32, screenshotInfo.OnRenderedEvent, true );
            }

            --screenshotInfo.RemainingFrames;
        }
    }
}

// ***********************
//
void    OutputScreenshot::ProcessFrameData  ( NRenderContext * ctx, RenderedChannelsData * rcd )
{
    for( auto & screenshotInfo : m_screenshotDescs )
    {
        ProcessFrameData( screenshotInfo, ctx, rcd->GetRenderChannel( screenshotInfo.InputChannel ) );
    }

    RemoveFinalizedRequests();
}

// ***********************
// Next frame will be written to file.
void    OutputScreenshot::RequestScreenshot ( const std::string & filePath, RenderChannelType channelId, unsigned int numFrames, bool onRenderedEvent, bool saveImgAsync )
{
    ScreenshotInfo desc;
    desc.InputChannel = channelId;
    desc.FilePath = filePath;
    desc.OnRenderedEvent = onRenderedEvent;
    desc.SaveToFileAsync = saveImgAsync;
    desc.RemainingFrames = numFrames;
    desc.AllFrames = numFrames;

    // Create directory for future use if not exists.
    Path file( filePath );
    auto directory = file.ParentPath();

    if( directory != "" && !Dir::Exists( directory.Str() ) )
        Dir::CreateDir( directory.Str(), true );

    m_screenshotDescs.push_back( std::move( desc ) );
}

// ***********************
//
bool    OutputScreenshot::ReadbackNeeded  ( ScreenshotInfo & screenshotInfo )
{
    return screenshotInfo.RemainingFrames > 0 ? true : false;
}

// ***********************
//
void    OutputScreenshot::RemoveFinalizedRequests()
{
    for( auto iter = m_screenshotDescs.begin(); iter != m_screenshotDescs.end(); )
    {
        if( iter->RemainingFrames <= 0 )
        {
            iter = m_screenshotDescs.erase( iter );
        }
        else
        {
            ++iter;
        }
    }
}



} //nrl
} //bv
