#pragma once

#include "AudioBuffer.h"
#include "Engine/Audio/AudioRenderChannelData.h"

namespace bv { namespace audio
{

class AudioMixer
{
    const audio::AudioRenderChannelData & m_arcd;

public:
    explicit            AudioMixer          ( const audio::AudioRenderChannelData & arcd );

    bool                MixAudioBuffersVecs ( const MemoryChunkPtr & output ) const;
    bool                IsAnyBufferReady    ( SizeType requestedBufferSize ) const;

private:
    void                MixAudioBuffers     ( const MemoryChunkPtr & output, const AudioBufferVec & audioBuffs ) const;

    AudioMixer & operator= ( const AudioMixer & );
};

} // audio
} // bv