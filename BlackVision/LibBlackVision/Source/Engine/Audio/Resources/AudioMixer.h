#pragma once

#include "AudioBuffer.h"
#include "Engine/Audio/AudioRenderChannelData.h"
#include "DataTypes/Deque.h"

namespace bv { namespace audio
{

class AudioMixer
{
    std::vector< Deque< MemoryChunkConstPtr > > m_audioBuffersSources;
    std::vector< SizeType >                     m_sizes;
    std::vector< bool >                         m_eofVec;

    Float32			    m_gain;

public:
    explicit            AudioMixer          ();

    bool                PopAndMixAudioData  ( const MemoryChunkPtr & output );
    bool                IsAnyBufferReady    ( SizeType requestedBufferSize ) const;

    SizeType            NumSources          () const;
    void                ResizeSources       ( SizeType numSources );
    void                PushData            ( SizeType sourceIdx, const AudioBufferVec & audioBuffs );

    void                SetGain             ( Float32 gain );

private:
    bool                MixAudioBuffers     ( const MemoryChunkPtr & output, SizeType sourceIdx );

    AudioMixer & operator= ( const AudioMixer & );
};

} // audio
} // bv