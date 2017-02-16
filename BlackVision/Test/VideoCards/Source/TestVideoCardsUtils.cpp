#include "TestVideoCardsUtils.h"
#include "Assets/Texture/TextureUtils.h"

namespace bv { namespace videocards {

// ****************************
//
AVFramePtr   TestVideoCardsUtils::CreateTestFrame( int color, UInt32 w, UInt32 h )
{
    AVFrameDescriptor desc;

    desc.width  = w;
    desc.height = h;
    desc.depth  = TextureUtils::Channels( TextureFormat::F_A8R8G8B8 );
    desc.channels = 0;
    desc.sampleRate = 0;
    desc.fieldModeEnabled       = true;
    desc.timeCodePresent        = true;
    desc.autoGenerateTimecode   = true;

    MemoryChunkPtr audioData = MemoryChunk::Create( 1 );

    auto size = w * h * 4;

    MemoryChunkPtr videoData = MemoryChunk::Create( size );

    auto writable = videoData->GetWritable();

    memset( writable, color, size );

    return std::make_shared< videocards::AVFrame >( videoData, audioData, desc );
}

} // videocards
} // bv