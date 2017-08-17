#pragma once

#include "CoreDEF.h"
#include "Memory/MemoryChunk.h"

#include "Engine/Audio/Resources/AudioBuffer.h"

namespace bv { namespace audio
{

class AudioRenderChannelData
{
    std::vector< AudioBufferVec >   m_audioBuffers;
    std::vector< SizeType >         m_sizes;

public:
    void                            ClearBuffers    ();

    void                            PushData        ( const AudioBufferVec & chunks );
    void                            PushData        ( const AudioBufferVec &, SizeType sourceIdx );

    SizeType                        NumSources      () const;

    AudioBufferVec                  GetData         ( SizeType sourceIdx ) const;

    SizeType                        DataSize        ( SizeType sourceIdx ) const;
};

} // audio
} // bv