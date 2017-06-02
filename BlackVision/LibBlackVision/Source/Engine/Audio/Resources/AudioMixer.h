#pragma once

#include "AudioBuffer.h"
#include "Engine/Audio/AudioRenderChannelData.h"

namespace bv { namespace audio
{

class AudioMixer
{
    const audio::AudioRenderChannelData & m_arcd;

    Float32			    m_gain;

public:
    explicit            AudioMixer          ( const audio::AudioRenderChannelData & arcd );

    bool                MixAudioBuffersVecs ( const MemoryChunkPtr & output ) const;
    bool                IsAnyBufferReady    ( SizeType requestedBufferSize ) const;

    void                SetGain             ( Float32 gain );

private:
    void                MixAudioBuffers     ( const MemoryChunkPtr & output, const AudioBufferVec & audioBuffs ) const;

    AudioMixer & operator= ( const AudioMixer & );
};

} // audio
} // bv