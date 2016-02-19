#include "stdafx.h"

#include "ScreenShotLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "LibImage.h"

#include "System/Path.h"
#include "IO/DirIO.h"
#include "Tools/Profiler/HerarchicalProfiler.h"

namespace bv
{

// ***********************
//
ScreenShotLogic::ScreenShotLogic( unsigned int numReadBuffers )
    :   m_remainingFrames( 0 ),
        m_allFrames( 0 ),
        m_curReadbackFrame( 0 )
{ m_asyncWrites.resize( numReadBuffers ); }

// ***********************
//
ScreenShotLogic::~ScreenShotLogic()
{}

// ***********************
// Next frame will be written to file.
void ScreenShotLogic::MakeScreenShot( const std::string& filePath, unsigned int numFrames )
{
    if( m_remainingFrames > 0 ) // Can't make new screenshot, while recording frames.
        return;

    m_filePath = filePath;

    Path file( filePath );
    auto directories = file.Split();

    // Create directory for screenshots
    std::string directory = "";
    for( int i = 0; i < directories.size() - 1; ++i )
        directory += directories[ i ];

    if( directory != "" && !Dir::Exists( directory ) )
        Dir::CreateDir( directory, true );

    m_remainingFrames = numFrames;
    m_allFrames = numFrames;
}

// ***********************
//
void ScreenShotLogic::FrameRendered   (  Renderer* renderer, OffscreenRenderLogic* offscreenRenderLogic )
{
    HPROFILER_SECTION( "ScreenShot Logic", PROFILER_THREAD1 );
    
    //FIXME
    { renderer; offscreenRenderLogic; }
    assert( false );

    //offscreenRenderLogic->DrawWithAllVideoEffects( renderer );
    //offscreenRenderLogic->DrawDisplayRenderTarget( renderer );

    //if( ReadbackNeeded() )
    //{
    //    std::string newFilePath = m_filePath + std::to_string( m_allFrames - m_remainingFrames ) + ".bmp";

    //    Texture2DConstPtr frame;
    //    {
    //        HPROFILER_SECTION( "Frame Readback", PROFILER_THREAD1 );
    //        if( m_asyncWrites[ m_curReadbackFrame ].valid() )
    //            m_asyncWrites[ m_curReadbackFrame ].get();      // Wait until frame will be written.

    //        frame = offscreenRenderLogic->ReadDisplayTarget( renderer, m_curReadbackFrame );
    //        m_curReadbackFrame = ( m_curReadbackFrame + 1 ) % offscreenRenderLogic->NumReadBuffersPerRT();
    //    }

    //    {
    //        HPROFILER_SECTION( "Write to file", PROFILER_THREAD1 );
    //        auto chunk = frame->GetData();

    //        m_asyncWrites[ m_curReadbackFrame ] = std::async( image::SaveBMPImage, newFilePath, chunk, frame->GetWidth(), frame->GetHeight(), 32 );

    //        --m_remainingFrames;
    //    }
    //}
    //offscreenRenderLogic->SwapDisplayRenderTargets();
}

// ***********************
//
bool ScreenShotLogic::ReadbackNeeded  ()
{    return m_remainingFrames > 0 ? true : false;   }

} //bv
