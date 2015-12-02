#include "ScreenShotLogic.h"

#include "Rendering/OffscreenRenderLogic.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "LibImage.h"

#include "System/Path.h"
#include "IO/DirIO.h"

namespace bv
{

// ***********************
//
ScreenShotLogic::ScreenShotLogic()
    :   m_remainingFrames( 0 ),
        m_allFrames( 0 )
{}

// ***********************
//
ScreenShotLogic::~ScreenShotLogic()
{}

// ***********************
// Next frame will be written to file.
void ScreenShotLogic::MakeScreenShot( const std::string& filePath, unsigned int numFrames )
{
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
    
    //offscreenRenderLogic->DrawWithAllVideoEffects( renderer );
    offscreenRenderLogic->DrawDisplayRenderTarget( renderer );

    if( ReadbackNeeded() )
    {
        std::string newFilePath = m_filePath + std::to_string( m_allFrames - m_remainingFrames ) + ".bmp";

        auto frame = offscreenRenderLogic->ReadDisplayTarget( renderer, 0 );

        auto chunk = frame->GetData();
        image::SaveBMPImage( newFilePath, chunk, frame->GetWidth(), frame->GetHeight(), 32 );

        --m_remainingFrames;
    }
    offscreenRenderLogic->SwapDisplayRenderTargets();
    //offscreenRenderLogic->SwapDisplayRenderTargets();
}

// ***********************
//
bool ScreenShotLogic::ReadbackNeeded  ()
{    return m_remainingFrames > 0 ? true : false;   }

} //bv
