#include "stdafx.h"

#include "ScreenShotLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "LibImage.h"

#include "System/Path.h"
#include "IO/DirIO.h"
#include "Tools/Profiler/HerarchicalProfiler.h"

#include "Engine/Events/EventManager.h"
#include "Engine/Events/Events.h"

namespace bv
{

// ***********************
//
void SaveImageToFileFunction    ( const std::string & filePath, MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp, bool sendEvent )
{
    bool result = image::SaveBMPImage( filePath, data, width, height, bpp );

    if( sendEvent )
    {
        ScreenShotRenderedEventPtr renderedEvent = std::make_shared< ScreenShotRenderedEvent >();
        renderedEvent->FilePath = filePath;
        renderedEvent->Result = result;

        GetDefaultEventManager().QueueEvent( renderedEvent );
    }
}

// ***********************
//
ScreenShotLogic::ScreenShotLogic( unsigned int numReadBuffers )
    :   m_remainingFrames( 0 ),
        m_allFrames( 0 ),
        m_curReadbackFrame( 0 ),
        m_onRenderedEvent( false )
{ m_asyncWrites.resize( numReadBuffers ); }

// ***********************
//
ScreenShotLogic::~ScreenShotLogic()
{}

// ***********************
// Next frame will be written to file.
void ScreenShotLogic::MakeScreenShot( const std::string& filePath, unsigned int numFrames, bool onRenderedEvent )
{
    if( m_remainingFrames > 0 ) // Can't make new screenshot, while recording frames.
        return;

    m_onRenderedEvent = onRenderedEvent;

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
void ScreenShotLogic::FrameRendered   ( RenderTarget * renderTarget, RenderLogicContext * ctx )
{
    HPROFILER_SECTION( "ScreenShot Logic", PROFILER_THREAD1 );

    if( ReadbackNeeded() )
    {
        std::string newFilePath = m_filePath + std::to_string( m_allFrames - m_remainingFrames ) + ".bmp";

        Texture2DPtr frame;
        {
            HPROFILER_SECTION( "Frame Readback", PROFILER_THREAD1 );

            renderer( ctx )->ReadColorTexture( 0, renderTarget, frame );
        }

        {
            HPROFILER_SECTION( "Write to file", PROFILER_THREAD1 );
            auto chunk = frame->GetData();

            m_asyncWrites[ m_curReadbackFrame ] = std::async( SaveImageToFileFunction, newFilePath, chunk, frame->GetWidth(), frame->GetHeight(), 32, m_onRenderedEvent );

            --m_remainingFrames;
        }
    }
}

// ***********************
//
bool ScreenShotLogic::ReadbackNeeded  ()
{    return m_remainingFrames > 0 ? true : false;   }

} //bv
