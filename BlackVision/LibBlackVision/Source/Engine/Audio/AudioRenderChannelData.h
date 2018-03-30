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

    // FIXME: This is hack. We must disable sending all channels to preview. Find better solution.
    bool                            m_sendToPreview;

public:
    void                            ClearBuffers    ();

    void                            PushData        ( const AudioBufferVec & chunks );
    void                            PushData        ( const AudioBufferVec &, SizeType sourceIdx );

    SizeType                        NumSources      () const;

    AudioBufferVec                  GetData         ( SizeType sourceIdx ) const;

    SizeType                        DataSize        ( SizeType sourceIdx ) const;

    bool                            SendToPreview   () const { return m_sendToPreview; }
    void                            SendToPreview   ( bool enable ) { m_sendToPreview = enable; }
};

} // audio
} // bv