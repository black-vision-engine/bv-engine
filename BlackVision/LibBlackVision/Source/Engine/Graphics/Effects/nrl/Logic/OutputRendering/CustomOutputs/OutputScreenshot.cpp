#include "stdafx.h"

#include "OutputScreenshot.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"

#include "LibImage.h"

#include "System/Path.h"
#include "IO/DirIO.h"
#include "Tools/Profiler/HerarchicalProfiler.h"

#include "Engine/Events/EventManager.h"
#include "Engine/Events/Events.h"



namespace bv { namespace nrl {


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
OutputScreenshot::OutputScreenshot( unsigned int width, unsigned int height )
    :   OutputInstance( width, height )
    ,   m_remainingFrames( 0 )
    ,   m_allFrames( 0 )
    ,   m_onRenderedEvent( false )
    ,   m_saveToFileAsync( true )
{}

// *********************************
//
void    OutputScreenshot::ProcessFrameData  ( NRenderContext * ctx, RenderResult * input )
{
    if( ReadbackNeeded() )
    {
        std::string newFilePath = m_filePath + std::to_string( m_allFrames - m_remainingFrames ) + ".bmp";

        auto rct = GetActiveRenderChannel();
        assert( input->IsActive( rct ) && input->ContainsValidData( rct ) );

        Texture2DPtr frame;
        renderer( ctx )->ReadColorTexturSync( 0, input->GetActiveRenderTarget( rct ), frame );

        auto chunk = frame->GetData();

        if( m_saveToFileAsync )
        {
            std::thread asyncWrite( SaveImageToFileFunction, newFilePath, chunk, frame->GetWidth(), frame->GetHeight(), 32, m_onRenderedEvent, false );
            asyncWrite.detach();
        }
        else
        {
            SaveImageToFileFunction( newFilePath, chunk, frame->GetWidth(), frame->GetHeight(), 32, m_onRenderedEvent, true );
        }

        --m_remainingFrames;
    }
}


// ***********************
// Next frame will be written to file.
void    OutputScreenshot::MakeScreenShot    ( const std::string & filePath, unsigned int numFrames, bool onRenderedEvent, bool saveImgAsync )
{
    if( m_remainingFrames > 0 ) // Can't make new screenshot, while recording frames.
        return;

    m_onRenderedEvent = onRenderedEvent;
    m_saveToFileAsync = saveImgAsync;

    m_filePath = filePath;

    Path file( filePath );
    auto directory = file.ParentPath();

    if( directory != "" && !Dir::Exists( directory.Str() ) )
        Dir::CreateDir( directory.Str(), true );

    m_remainingFrames = numFrames;
    m_allFrames = numFrames;
}

// ***********************
//
bool    OutputScreenshot::ReadbackNeeded  ()
{
    return m_remainingFrames > 0 ? true : false;
}



} //nrl
} //bv
