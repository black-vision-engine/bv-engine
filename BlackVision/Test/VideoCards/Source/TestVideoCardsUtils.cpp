#include "TestVideoCardsUtils.h"
#include "Assets/Texture/TextureUtils.h"

#include <windows.h>

namespace bv { namespace videocards {

// ****************************
//
AVFramePtr   TestVideoCardsUtils::CreateTestFrame   ( int color, UInt32 w, UInt32 h )
{
    AVFrameDescriptor desc;

    desc.width  = w;
    desc.height = h;
    desc.depth  = TextureUtils::Channels( TextureFormat::F_A8R8G8B8 );
    desc.channels = 0;
    desc.sampleRate = 0;
    //desc.fieldModeEnabled       = true;  // FIXME: https://www.pivotaltracker.com/story/show/145508031
    //desc.timeCodePresent        = true;
    //desc.autoGenerateTimecode   = true;

    MemoryChunkPtr audioData = MemoryChunk::Create( 1 );

    auto size = w * h * 4;

    MemoryChunkPtr videoData = MemoryChunk::Create( size );

    auto writable = videoData->GetWritable();

    memset( writable, color, size );

    return std::make_shared< AVFrame >( videoData, audioData, desc );
}

// ****************************
//
bool        TestVideoCardsUtils::UpdateConsoleTitle     ( const std::wstring title )
{
    SetConsoleTitle( title.c_str() );
    return true;
}

// ****************************
//
void        TestVideoCardsUtils::SetFPS                ( UInt64 frameTime )
{
    static Float32 lastFrameTime = 1000.f / frameTime;
    static bool dummy = UpdateConsoleTitle( std::to_wstring( 1000.f / frameTime ) + L" FPS" );

    Float32 newFPS = 1000.f / frameTime;

    if( std::abs( newFPS - lastFrameTime ) > 0.1f )
    {
        lastFrameTime = newFPS;
        UpdateConsoleTitle( std::to_wstring( lastFrameTime ) + L" FPS" );
    }
}

} // videocards
} // bv